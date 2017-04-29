#ifndef UGEMM_HPP
#define UGEMM_HPP
#include "utp.hpp"
#ifdef WITH_MKL
#include "mkl.h"
#else
#include "gsl/gsl_cblas.h"
#endif

#include <cublas.h>

namespace utp{
  namespace ublas{
    /*===================================================================================*/
    class Gemm: public OperationBase<Gemm>{
    public:
      Gemm(string n){this->name = n;};
      string get_name(){return name;}
      /*------------------------------------------------------------*/
      template <typename Scheduler,typename P>
      static inline void split(Scheduler &s,Task<Gemm,P> *t);
      /*------------------------------------------------------------*/
      template <typename P>
      static inline void run(Task<Gemm,P> *t){
#if UTP_DEBUG != 0 
	cout << "----\tGemm.run\tgemm_"<< t->id << endl;
#     endif

	GData *a = t->args->args[0];
	GData *b = t->args->args[1];
	GData *c = t->args->args[2];


	double *A=(double *)a->get_memory();
	int ldA = a->get_rows();
	int M = a->get_rows();

	double *B=(double *)b->get_memory();
	int ldB = b->get_rows();
	int N = b->get_cols();

	double *C=(double *)c->get_memory();
	int ldC = c->get_rows();
	int K = a->get_cols();
	double beta= t->beta; 
	double alpha= t->alpha;
	auto TransA = CblasNoTrans;
	auto TransB = t->trans_b?CblasTrans:CblasNoTrans;
	//Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)

      
      cblas_dgemm(CblasColMajor,TransA,TransB,M,N,K,alpha,A,ldA,B,ldB,beta,C,ldC);
#     if UTP_DEBUG != 0 
      cout << "call blasDgemm() for " << c->get_name() << endl << flush;
      cout << M << "," << N << "," << K << "," << A << "," << ldA << "," << B << "," << ldB << "," << C << "," << ldC << endl;
      cout << "-----------\n" ;
#     endif
#if SHORTCUT == 0 
      Dispatcher::finished(t);
#endif
    }
    /*---------------------------------------------------------------*/
      template <typename P>
      static void run_on_gpu(Task<Gemm,P> *t){
	GData *a = t->args->args[0];
	GData *b = t->args->args[1];
	GData *c = t->args->args[2];


	//	double *A=(double *)a->get_memory();
	double *A = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[0]);
	
	int ldA = a->get_rows();
	int M = a->get_rows();

	//	double *B=(double *)b->get_memory();
	double *B = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[1]);
	int ldB = b->get_rows();
	int N = b->get_cols();
	
	//	double *C=(double *)c->get_memory();
	double *C = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[2]);
	int ldC = c->get_rows();
	int K = a->get_cols();
	double beta= 1.0; 
	double alpha= 1.0;
	auto TransA = CblasNoTrans;
	#if UTP_DEBUG!=0
	cout << "call cublasDgemm() for " << c->get_name() << endl << flush;
	#endif
	cublasDgemm( 'n', 'n', K, M, M, alpha, A, ldA, B, ldB, beta, C, ldC);
	Dispatcher::finished(t);
      }
  };
  /*===================================================================================*/
  extern Gemm gemm_instance;
  extern unsigned int gemm_task_count;
  /*===================================================================================*/
  template <typename P>
  class GemmTask: public Task<Gemm,P>{
  public:
    static const int gemm_type_id=0;
    /*------------------------------------------------------------*/
    Task<Gemm,P>*getHost(){
      return this;
    }
    /*------------------------------------------------------------*/
    GemmTask(GData &A,GData &B,GData &C,P *p=nullptr):Task<Gemm,P>(&gemm_instance){
      this->args = new Args;      
      this->axs = new Axs;
      packArgs( this->args, A , B  , C    );
      packAxs (*this->axs , In, In , InOut);
      this->id = gemm_task_count ++;
      this->type_id = gemm_type_id;
      this->child_count=0;
      set_parent(p);
    }
    /*------------------------------------------------------------*/
    ~GemmTask(){
      delete this->args;
      delete this->axs;
    }
  };
  /*===================================================================================*/
  void ugemm(GData &,GData &, GData &);
  template<typename S,typename Oper,typename P>
  void ugemm(S &s,GData &A,GData &B, GData &C,Task<Oper,P >* p=nullptr);

  /*===================================================================================*/
  template <typename Scheduler,typename P>
  void Gemm::split(Scheduler &s,Task<Gemm,P> *task){

#if UTP_DEBUG !=0 
    cout << s.name <<"\tGemm.split\t" << task->o->name <<"_" << task->id << endl;
#endif
    GData &A = *task->args->args[0];
    GData &B = *task->args->args[1];
    GData &C = *task->args->args[2];
    int m = A.get_part_countX();
    int n = B.get_part_countX();
    int o = C.get_part_countX();
    //cout << "&&&&&&&" << m <<"," << n << "," << o << endl;
    for ( int i=0;i<m;i++){
      for (int j=0;j<n;j++){
	for (int k=0;k<o;k++){
	  GemmTask<Task<Gemm,P>> *t = new GemmTask<Task<Gemm,P>>(A(i,k),B(k,j),C(i,j),task);
	  Dispatcher::submit(s,t);
	}
      }
    }
  }
} // ublas namespace
} // utp namespace
#endif // UGEMM_HPP
