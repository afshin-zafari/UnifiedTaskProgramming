#include "gsl/gsl_cblas.h"
/*----------------------------------------------------------------------------*/
struct GemmTask : public SuperGlueTaskBase {
  bool b_trans,c_decrease;
/*----------------------------------------------------------------------------*/
  GemmTask(IDuctteipTask *task_ ,
	   Handle<Options> &h1,
	   Handle<Options> &h2,
	   Handle<Options> &h3,
	   bool trans_b=false,
	   bool decrease_c = false):
    SuperGlueTaskBase (task_),
    b_trans(trans_b),
    c_decrease(decrease_c){
        registerAccess(ReadWriteAdd::read , h1);
        registerAccess(ReadWriteAdd::read , h2);
        registerAccess(ReadWriteAdd::write, h3);
    }

/*----------------------------------------------------------------------------*/
    void runKernel() {
      int N = getAccess(1).getHandle()->block->X_E();
      double *mA = getAccess(1).getHandle()->block->getBaseMemory();
      double *mB = getAccess(2).getHandle()->block->getBaseMemory();
      double *mC = getAccess(3).getHandle()->block->getBaseMemory();
      char transb = (b_trans)?'T':'N';
      double alpha = (c_decrease)?-1.0:1.0;
      double beta = 1.0;
      auto TransA = CblasNoTrans;
      //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
      cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,N,N,N,alpha,mA,N,mB,N,beta,mC,N);
      //dgemm('N',transb,N,N,N,alpha,a,N,b,N,beta,c,N);
    }
  /*----------------------------------------------------------------------------*/
};
