#ifndef USYRK_HPP
#define USYRK_HPP
#include "utp.hpp"
#include "ugemm.hpp"

#ifdef WITH_MKL
#include "mkl.h"
#else
#include "gsl/gsl_cblas.h"
#endif
#include <cublas.h>

namespace utp{
  namespace ublas{
    /*===================================================================================*/
    class Syrk: public OperationBase<Syrk>{
    public:
      Syrk(string n){this->name = n;};
      string get_name(){return name;}
      /*------------------------------------------------------------*/
      template <typename Scheduler,typename P>
      static inline void split(Scheduler &s,Task<Syrk,P> *t);
      /*------------------------------------------------------------*/
      template <typename P>
      static inline void run(Task<Syrk,P> *t){

	GData *a = t->args->args[0];
	GData *b = t->args->args[1];


	double *A=(double *)a->get_memory();
	int ldA = a->get_rows();
	int N = a->get_rows();

	double *B=(double *)b->get_memory();
	int ldB = b->get_rows();
	int K = b->get_cols();

	cblas_dsyrk (CblasColMajor, CblasLower, CblasNoTrans,N, K, 1.0, A, ldA, 1.0, B, ldB);


      
	Dispatcher::finished(t);
      }
      /*---------------------------------------------------------------*/
      template <typename P>
      static void run_on_gpu(Task<Syrk,P> *t){
	GData *a = t->args->args[0];
	GData *b = t->args->args[1];


	//	double *A=(double *)a->get_memory();
	double *A = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[0]);
	
	int ldA = STARPU_MATRIX_GET_LD(t->gpuArgs[0]);//a->get_rows();
	int N = a->get_rows();

	//	double *B=(double *)b->get_memory();
	double *B = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[1]);
	int ldB = STARPU_MATRIX_GET_LD(t->gpuArgs[1]);//b->get_rows();
	int K = b->get_cols();
	
	//	cublasDsyrk ( uplo,  trans, n, k, alpha, *A, lda, beta, C,  ldc);
	cublasDsyrk ('L', 'N', N, K, 1.0,A,ldA,1.0,B,ldB);
	Dispatcher::finished(t);
      }
    };
    /*===================================================================================*/
    extern Syrk syrk_instance;
    extern unsigned int syrk_task_count;
    /*===================================================================================*/
    template <typename P>
    class SyrkTask: public Task<Syrk,P>{
    public:
      static const int syrk_type_id=4;
      /*------------------------------------------------------------*/
      Task<Syrk,P>*getHost(){
	return this;
      }
      /*------------------------------------------------------------*/
      SyrkTask(GData &A,GData &B,P *p=nullptr):Task<Syrk,P>(&syrk_instance){
	this->args = new Args;      
	this->axs = new Axs;
	packArgs( this->args, A , B    );
	packAxs (*this->axs , In, InOut);
	this->id = syrk_task_count ++;
	this->type_id = syrk_type_id;
	this->child_count=0;
	set_parent(p);
      }
      /*------------------------------------------------------------*/
      ~SyrkTask(){
	delete this->args;
	delete this->axs;
      }
    };
    /*===================================================================================*/
    void usyrk(GData &,GData &);
    template<typename S,typename Oper,typename P>
    void usyrk(S &s,GData &A,GData &B,Task<Oper,P >* p=nullptr);

    /*===================================================================================*/
    template <typename Scheduler,typename P>
    void Syrk::split(Scheduler &s,Task<Syrk,P> *task){

#if UTP_DEBUG !=0 
      cout << s.name <<"\tSyrk.split\t" << task->o->name <<"_" << task->id << endl;
#endif
      GData &A = *task->args->args[0];
      GData &B = *task->args->args[1];
      int n = A.get_part_countX();
      for(int i = 0; i<n ; i++){
	for(int j = 0; j<i+1; j++){
	  for(int k = 0; k<n; k++){
	    if ( i ==j ){
	      SyrkTask<Task<Syrk,P>> *syrk = new SyrkTask<Task<Syrk,P>>(A(i,k),B(i,j),task);
	      Dispatcher::submit(s,syrk);
	    }
	    else{
	      GemmTask<Task<Syrk,P>> *gemm=new GemmTask<Task<Syrk,P>>(A(i,k),A(j,k),B(i,j),task);
	      gemm->trans_b = true;
	      Dispatcher::submit(s,gemm);
	    }
	  }
	}
      }
    }
  } // ublas namespace
} // utp namespace

#endif
