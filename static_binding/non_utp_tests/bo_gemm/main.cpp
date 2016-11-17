#include <iostream>
#include "basic.hpp"
#include "cmdline.hpp"
#include "gsl/gsl_cblas.h"
using namespace std;

int N, B1;
void fill_rows_with(double *X, double v){
  for(int i=0; i< N*N;i++)
    X[i] = v;
}
void gemm(double *A, double *B, double *C){

  int Bz  = N / B1 ; 
  int ldA = Bz;
  int M   = Bz;

  int ldB = Bz;
  int N   = Bz;

  int ldC = Bz;
  int K   = Bz;
  double beta= 1.0; 
  double alpha= 1.0;
  auto TransA = CblasNoTrans;
  //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
  cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,M,N,K,alpha,A,ldA,B,ldB,beta,C,ldC);
}
#define block(X,i,j) X + Bz*Bz*(B1*j+i)
void gemm_block(double *A, double *B, double *C){
  int Bz = N / B1;
  for ( int i=0;i<B1;i++){
    for (int j=0;j<B1;j++){
      for (int k=0;k<B1;k++){
	double * bA =block(A,i,k);
	double * bB =block(B,k,j);
	double * bC =block(C,i,j);
	gemm(bA,bB,bC);	
      }
    }
  }
}
int main(int argc, char *argv[])
{
    cout << "Starts\n";
    cmdLine.getCmdLine(argc,argv);
    N  = cmdLine.M;
    B1 = cmdLine.Mb;
    double *A,*B,*C;
    A = new double [N*N];
    B = new double [N*N];
    C = new double [N*N];
    fill_rows_with(A,1);
    fill_rows_with(B,2);
    fill_rows_with(C,0);
    TimeUnit start = UserTime();
    gemm_block(A,B,C);
    TimeUnit end = UserTime();

    cout << "Finished\n";
    cout << "execution time in ms (BLAS ONLY without UTP): " << (end - start)/1000.0 << endl;
   
    double sum = 0.0;
    for(int i=0;i<N*N;i++){
      sum += C[i];
    }
    cout << " sum of C: " <<sum << endl;
    
    delete [] A;
    delete [] B;
    delete [] C;
    
    return 0;
}

