#include <iostream>
#include "unified_blas.hpp"
#include "Dispatcher.hpp"
#include "ductteip.hpp"
#include "config.hpp"
#include <map>
#include <cstdio>
#include <sys/time.h>

map<string,GOperation *> op;
using namespace std;
timeval start,finish;
void tic()
{
    gettimeofday(&start,NULL);
}
double toc()
{
    gettimeofday(&finish,NULL);
    return ((finish.tv_sec-start.tv_sec)*1000000+finish.tv_usec-start.tv_usec)/1000000.0;
}
/*
  main:
    config 1
      A << lu_p << lu_p     << ( col_p       + blk_p        + row_p) ; 
          A(0-4)   A(0)(0-4)    A(0)(0)(0:n)   A(0)(:)(m,n)   A(0)(1:4)(0:m)
    config 2 
      A  << lu_p << ( col_p + blk_p + row_p) ; 
    lupp(A,piv)
  --------------------------
  lupp A,piv:
  non-leaf: ( if A.part == lu_p)
    Alu=A<<lu_part  
    LUPP Alu(0) piv            oper=lupp
    SWAP piv    Alu(1) Alu(2)  oper=swap
    SWAP piv    Alu(3) Alu(4)  oper=swap
    TRSM Alu(0) Alu(1)         oper=trsm
    GEMM Alu(0) Alu(1) Alu(2)  oper=gemm
    TRSM Alu(0) Alu(3)         oper=trsm
    GEMM Alu(0) Alu(3) Alu(4)  oper=gemm
  leaf: ( if A.part == col_p)
    Acol = A<<col_part
    Ablk = A<<blk_part
    j=1:Acol.N
      GETRF Acol(j)     piv                       oper=getrf2_kernel
       SWAP piv         Ablk(j,k)    Ablk(l,k)    oper=swap  k=j+1:N, l=j+1:M
    or SWAP piv         Apiv(0)      Apiv(1)      oper=swap Apiv is superset of Ablk
       TRSM Ablk(j,j)   Ablk(j,j+1)               oper=trsm
       GEMM Ablk(k,j)   Ablk(j,j+1)  Ablk(k,j+1)  oper=gemm  k=j+1:M
       TRSM Ablk(j,j)   Ablk(j,k)                 oper=trsm  k=j+2:N
       GEMM Ablk(j,k)   Ablk(l,k)    Ablk(l,k)    oper=gemm  k=j+2:N  l=j+1:M
---------------------------------
  swap piv, A, B:
    if B.part has/can row_p:
      Ar = A<<row_p
      Br = B<<row_p
      TempData T
      for i=1:len(piv)
        swap_? Ar(i), Br(piv(i)),T, parent_task = this_task   // T makes sure that pivot is done in the same order as they have been made in original getrf2
    elif B.part has/can blk_p:
      Ab = A << blk_p
      Bb = B << blk_p
      for i=1:len(piv)
        for ii=1:Ab.M
          if i in Ab(ii,0):
            for j=1:Bb.M
  	      if piv(i) in Bb(j,0):
                for k=1:Bb.N
	          swap_?? Ab(ii,k),Bb(j,k), i,piv(i)

 */
class LUPartitioner: public GPartitioner
{
public:
  LUPartitioner(int n , int s){}  
};
class ColumnPartitioner: public GPartitioner
{
public:
  ColumnPartitioner(int n , int s){}
};
void swap_kernel(GData &piv,GData &A,GData &B){
  //   i: 0  1  2  3  4  5
  // piv: 2 10 13 15 16 18
  GView &Ar=*A.view(row_p);
  GView &Br=*B.view(row_p);
  for (int i=0;i<piv_len;i++){
    int swap_row = piv.get_data(i) ;
    bool my_row = B.owns(swap_row);
    if (my_row){
      swap_t(Ar(i),Br(swap_row));
    }
  }
}
void lu_trf(GData &A){
  int n = 10;
  int s = 10;
  ColumnPartitioner col_p(n,s);
  GPartitioner blk_p(MB,NB);  
  A.set_allocation_view(&col_p);
  A.set_comm_view(&blk_p);
 
  GView &Av=*A.view(&col_p);
  GView &a = A.view(&blk_p);
  
  for(int j=0;j<N;j++){
    getrf(A(j),piv(j));
    for (int i=j+1;i<M;i++){
      for(int k=j+1;k<N;k++){
	swap_t(piv(j),a(j,k),a(i,k));
      }
    }
    trsm(a(j,j),a(j,j+1));
    for (int i=j;i<M;i++){
      gemm(a(j,j+1),a(i,j),a(i,j+1));
    }
    for(int k=j+2; k<N;k++){
      trsm(a(j,j),a(j,k));
      for(int l=j+1;l<M;l++){
	gemm(a(j,k),a(l,j),a(l,k));
      }
    }
    
  }
    
}
const int MIN_BLOCK_SIZE=128;
void lu_trsm(GData &,GData &){}
void lu_gemm(GData &,GData &,GData&){}
void lupp(GData &A){
  int N=A.get_part_countX();
  int s=N/10;
  LUPartitioner p(N,s);
  A.set_partition(&p);
  if (s < MIN_BLOCK_SIZE){
    lu_trf(A);
  }
  else{
    lupp(A(0));
    lu_trsm(A(0),A(1));
    lu_gemm(A(0),A(1),A(2));
    lu_trsm(A(0),A(3));
    lu_gemm(A(0),A(3),A(4));
    lupp(A(-1));  // trailing matrix
  }
}
GData *test_gemm()
{
    int M = config.getYDimension();
    int YB1 = config.getYBlocks();
    int YB2 = config.getYLocalBlocks();
    GData *_A = new GData(M,M,"A");
    GData *_B = new GData(M,M,"B");
    GData *_C = new GData(M,M,"C");
    GData &A=*_A,&B=*_B,&C=*_C;

    GPartitioner *P1= new GPartitioner(YB1,YB1);
    GPartitioner *P2= new GPartitioner(YB2,YB2);
    P1->set_next(P2);
    cout << "+++++++++++++"<<
      A.get_name() << "," <<
      B.get_name() << "," <<
      C.get_name() << "." << endl;

    printf("Partitioning starts now %dx%d\n",YB1,YB1);

    A.set_partition(P1);
    B.set_partition(P1);
    C.set_partition(P1);
    A.fill_with(1.0);
    B.fill_with(2.0);
    cout << "+++++++++++++"<<endl;
    B.print();
    cout << "+++++++++++++"<<endl;
    C.fill_with(10.0);
    cout << "+++++++++++++"<<endl;
    tic();
    ugemm(A,B,C);
    return _C;
}
int main(int argc, char **argv)
{
    utp_initialize(argc,argv);


    utp_finalize();

    cout << "Unified Version. N: " << config.getYDimension()
        << ", B: " << config.getYBlocks()
        << ", time: " << toc()
        << endl;

    cout << "Program finished.\n";
    return 0;
}

