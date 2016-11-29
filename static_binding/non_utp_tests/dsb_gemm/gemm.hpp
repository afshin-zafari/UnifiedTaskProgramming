#include "gsl/gsl_cblas.h"
#include "sg/platform/atomic.hpp"
#include <cassert>
/*----------------------------------------------------------------------------*/
struct GemmTask : public Task<Options,-1> {
  bool b_trans,c_decrease;
  IDuctteipTask *task;
/*----------------------------------------------------------------------------*/
  GemmTask(IDuctteipTask *task_ ,
	   Handle<Options> &h1,
	   Handle<Options> &h2,
	   Handle<Options> &h3,
	   bool trans_b=false,
	   bool decrease_c = false):
    task (task_),
    b_trans(trans_b),
    c_decrease(decrease_c){
    Atomic::increase(&task->child_count);
    //cout << "++++++--- " << task->child_count  << endl;
        registerAccess(ReadWriteAdd::read , h1);
        registerAccess(ReadWriteAdd::read , h2);
        registerAccess(ReadWriteAdd::write, h3);
    }

/*----------------------------------------------------------------------------*/
  void run(TaskExecutor<Options> &te) {
      assert(getAccess(0).getHandle());
      assert(getAccess(0).getHandle()->block);
      assert(getAccess(1).getHandle());
      assert(getAccess(1).getHandle()->block);
      assert(getAccess(2).getHandle());
      assert(getAccess(2).getHandle()->block);
      
      int N =  getAccess(0).getHandle()->block->X_E();
      double *mA = getAccess(0).getHandle()->block->getBaseMemory();
      double *mB = getAccess(1).getHandle()->block->getBaseMemory();
      double *mC = getAccess(2).getHandle()->block->getBaseMemory();
      assert(mA);
      assert(mB);
      assert(mC);
      char transb = (b_trans)?'T':'N';
      double alpha = (c_decrease)?-1.0:1.0;
      double beta = 1.0;
      //cout << ">>>>" << N << endl;
      auto TransA = CblasNoTrans;
      //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
      cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,N,N,N,alpha,mA,N,mB,N,beta,mC,N);
    }
  /*----------------------------------------------------------------------------*/
  ~GemmTask(){    
    int count = Atomic::decrease_nv(&task->child_count);
    //cout << "-------- " << count  << endl;
    if ( count == 0 ) {
      string s = task->get_name();
      //      if ( task->isRunning() )
	task->setFinished(true);
	cout << "======================  " << s << endl;
    }
  }
  string get_name(){return "gemm";}
};
/*----------------------------------------------------------------------------*/
struct SyncTask : public Task<Options,-1> {
  bool b_trans,c_decrease;
  IDuctteipTask *task;
/*----------------------------------------------------------------------------*/
  SyncTask(IDuctteipTask *task_ ):task (task_){
        registerAccess(ReadWriteAdd::write , *task->getSyncHandle());
  }

/*----------------------------------------------------------------------------*/
    string get_name(){return "sync";}
    void run(TaskExecutor<Options> &te) {    }
  ~SyncTask(){
    task->setFinished(true);
  }
  /*----------------------------------------------------------------------------*/
};
