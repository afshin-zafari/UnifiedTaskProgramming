#ifndef UPOTRF_HPP
#define UPOTRF_HPP
#include "utp.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
#include "usyrk.hpp"

#ifdef WITH_MKL
#include "mkl.h"
#else
#include "gsl/gsl_cblas.h"
#include "lapacke.h"
#endif

#include <cublas.h>
#include <cusolverDn.h>

namespace utp{
  cusolverDnHandle_t *cusolver_handle;
  namespace ublas{
    /*===================================================================================*/
    class Potrf: public OperationBase<Potrf>{
    public:
      Potrf(string n){this->name = n;};
      string get_name(){return name;}
      /*------------------------------------------------------------*/
      template <typename Scheduler,typename P>
      static inline void split(Scheduler &s,Task<Potrf,P> *t);
      /*------------------------------------------------------------*/
      template <typename P>
      static inline void run(Task<Potrf,P> *t){
#if UTP_DEBUG != 0 
	cout << "----\tPOTRF.run\tgemm_"<< t->id << endl;
#     endif

	GData *a = t->args->args[0];

	double *A=(double *)a->get_memory();
	int ldA = a->get_rows();
	int M = a->get_rows();

      
      LAPACKE_dpotrf(CblasColMajor,'L',M,A,M);
#     if UTP_DEBUG != 0 
      cout << "call blasDpotrf() for " << a->get_name() << endl << flush;
#     endif
      Dispatcher::finished(t);
    }
    /*---------------------------------------------------------------*/
      template <typename P>
      static void run_on_gpu(Task<Potrf,P> *t){
	GData *a = t->args->args[0];


	//	double *A=(double *)a->get_memory();
	double *A = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[0]);
	
	int ldA = a->get_rows();
	int M = a->get_rows();

	cout << "call cublasDgemm() for " << a->get_name() << endl << flush;
	int lwork,dev_info;
	cusolverDnDpotrf_bufferSize(*cusolver_handle, CUBLAS_FILL_MODE_LOWER,M, A, M ,&lwork);
	if ( lwork){
	  double *work_space = new double[lwork];
	  cusolverDnDpotrf(*cusolver_handle,CUBLAS_FILL_MODE_LOWER,M, A, M,work_space,lwork,&dev_info);

	  if ( dev_info<0){
	    cout << "Error in cuSolver POTRF: " << -dev_info << endl;;
	    exit(dev_info);
	  }
	}
	Dispatcher::finished(t);
      }
  };
  /*===================================================================================*/
  extern Potrf potrf_instance;
  extern unsigned int potrf_task_count;
  /*===================================================================================*/
  template <typename P>
  class PotrfTask: public Task<Potrf,P>{
  public:
    static const int potrf_type_id=3;
    /*------------------------------------------------------------*/
    Task<Potrf,P>*getHost(){
      return this;
    }
    /*------------------------------------------------------------*/
    PotrfTask(GData &A,P *p=nullptr):Task<Potrf,P>(&potrf_instance){
      this->args = new Args;
      this->axs = new Axs;
      packArgs( this->args, A    );
      packAxs (*this->axs , InOut);
      this->id = potrf_task_count ++;
      this->type_id = potrf_type_id;
      this->child_count=0;
      this->set_parent(p);
    }
    /*------------------------------------------------------------*/
    ~PotrfTask(){
      delete this->args;
      delete this->axs;
    }
  };
  /*===================================================================================*/
  void upotrf(GData &);
  template<typename S,typename Oper,typename P>
  void upotrf(S &s,GData &A, Task<Oper,P >* p=nullptr);

  /*===================================================================================*/
    template <typename Scheduler,typename P>
    void Potrf::split(Scheduler &s,Task<Potrf,P> *task){

#if UTP_DEBUG !=0 
      cout << s.name <<"\tPotrf.split\t" << task->o->name <<"_" << task->id << endl;
#endif
      GData &A = *task->args->args[0];
      int n = A.get_part_countX();
      for(int i = 0; i<n ; i++){
	for(int j = 0; j<i ; j++){

	  SyrkTask<Task<Potrf,P>> *syrk = new SyrkTask<Task<Potrf,P>>( A(i,j), A(i,i),task);
	  Dispatcher::submit(s,syrk);
	  for (int k = i+1; k<n ; k++){
	    GemmTask<Task<Potrf,P>> *gemm=new GemmTask<Task<Potrf,P>>(A(k,j),A(i,j),A(k,i),task);
	    Dispatcher::submit(s,gemm);
	  }
	}
	PotrfTask<Task<Potrf,P>> *potrf = new PotrfTask<Task<Potrf,P>>(A(i,i),task);
	Dispatcher::submit(s,potrf);
	for( int j = i+1; j<n ; j++){

	  TrsmTask<Task<Potrf,P>> *trsm = new TrsmTask<Task<Potrf,P>>(A(i,i), A(j,i),task);
	  Dispatcher::submit(s,trsm);
	}
      }
    }
    /*--------------------------------------------------------------*/
  } // ublas namespace
} // utp namespace

#endif // UPOTRF_HPP
