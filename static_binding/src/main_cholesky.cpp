#include <iostream>
#include "utp.hpp"
#include "cmdline.hpp"
#include "upotrf.hpp"
//#include "starpu.h"

using namespace std;
size_t  N,B1,B2;
utp::GData *A,*B,*C;
utp::TimeUnit start;
utp::GPartitioner *P1,*P2;
#ifndef  DT_INCLUDED
int me;
#endif

/*---------------------------------------------*/
void mat_mult()
{
  N  = cmdLine.M;
  int B1 = cmdLine.Mb;
  int B2 = cmdLine.mb;
  A = new utp::GData(N,N,"A");
  B = new utp::GData(N,N,"B");
  C = new utp::GData(N,N,"C");
  P1 = new utp::GPartitioner(B1,B1);
  P2 = new utp::GPartitioner(B2,B2);
  P1->set_next(P2);
  A->set_partition(P1);
  B->set_partition(P1);
  C->set_partition(P1);

  A->fill_with(1.0,0.0);
  B->fill_with(2.0,0.0);
  C->fill_with(0.0,0.0);
  start = utp::UserTime();
  //test_DT_SPU_BLAS(*A,*B,*C);
}
/*---------------------------------------------*/
void mat_mult_test()
{
  A->print();
  B->print();
  C->print();
  if ( cmdLine.P  ==1 ) {
    double *c = (double *)C->get_memory();
    if (  c == nullptr)
      return  ;
    double sum = 0.0;
    for(int i=0;i<N*N;i++){
      sum += c[i];
    }
    cout << " sum of C: " <<sum << endl;
  }
  delete A;
  delete B;
  delete C;
}
/*---------------------------------------------*/

/*------------------------------------------*/
void cholesky()
{
  N  = cmdLine.M;
  B1 = cmdLine.Mb;
  B2 = cmdLine.mb;
  cout << "Define A"<< endl << flush ;
  A = new utp::GData (N,N,"A");
  P1 = new utp::GPartitioner(B1,B1);
  P2 = new utp::GPartitioner(B2,B2);
  #if DT_INCLUDED
  P1->set_next(P2);
  #endif
  A->set_partition(P1);
  cout << "After  partitioing A\n" << flush;
  #ifndef DT_INCLUDED
  //double *d = (double *)malloc(sizeof(double) * N*N);
  //  double *d = new double [(size_t)(N*N)];
  double *d=new double[N*N];
  //  starpu_malloc_flags((void **)&d,N*N,STARPU_MALLOC_PINNED);
  cout << "After mem allocation  A: " << d << "size: " << N*N << flush;
  A->set_memory(d,N);
  cout << "After mem allocation  A\n" << flush;
  A->fill_hilbert();
  #endif
  A->dump();
  cout << "After filling with\n" << flush;
  start = utp::UserTime();
  utp::ublas::upotrf(*A);
  //  test_DT_SPU_BLAS(*A,*B,*C);
  cout << "after task submission\n" << flush;  
}
/*---------------------------------------------*/
void cholesky_test(){
  A->dump();
  delete P1;
  delete P2;
  delete A;
}
/*---------------------------------------------*/
int main(int argc, char *argv[])
{
  cout << "Starts\n";

  utp::utp_initialize(argc,argv);
  cholesky();
  utp::utp_finalize();
  utp::TimeUnit end = utp::UserTime();
  cout << "Finished\n";
  cout << "[UTP] N=" << N << " , B1 = " << B1 <<" B2= " << B2 <<" , execution time in ms (D SPU B with UTP): " <<  (end - start) /1000.0 ;
  cout << " GFLOPS= " << N*N*N/3.0/1000.0/(end-start)<<endl;
  cholesky_test();
  return 0;
}

