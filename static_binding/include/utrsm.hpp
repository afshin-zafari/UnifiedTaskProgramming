#ifndef UTRSM_HPP
#define UTRSM_HPP
#include "utp.hpp"
#include "ugemm.hpp"

namespace utp{

namespace ublas{
    /*===================================================================================*/
    class Trsm: public OperationBase<Trsm>{
    public:
        using OperationBase<Trsm>::split;
        Trsm (string n){this->name = n;}
        string get_name(){return name;}
        template <typename Scheduler,typename P>
        static inline void split(Scheduler &s,Task<Trsm,P> *t);
      template <typename P>
      static inline void run(Task<Trsm,P> *t){
#         if UTP_DEBUG != 0
            cout << "----\tTrsm.run\ttrsm_"<< t->id << endl;
#         endif
	    GData *a = t->args->args[0];
	    GData *b = t->args->args[1];
	    PRINTF("trsm------------, B(%d) \n",b->get_child_index());


	    double *A=(double *)a->get_memory();
	    int ldA = a->get_rows();

	    double *B=(double *)b->get_memory();
	    int ldB = b->get_rows();
	    int M = b->get_rows();
	    int N = b->get_cols();

	    // Signature: cblas_dtrsm (Order, Side, Uplo, TransA, Diag, M, N, alpha, A, lda, B, ldb)
	    PRINTF("trsm------------, B(%d) \n",b->get_child_index());
	    cblas_dtrsm(CblasColMajor,CblasRight,CblasLower,CblasTrans,CblasUnit,M,N,1.0,A,ldA,B,ldB);
	    Dispatcher::finished(t);
        }
    /*---------------------------------------------------------------*/
      template <typename P>
      static void run_on_gpu(Task<Trsm,P> *t){
	GData *a = t->args->args[0];
	GData *b = t->args->args[1];


	//	double *A=(double *)a->get_memory();
	double *A = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[0]);
	
	int ldA = a->get_rows();
	int M = a->get_rows();

	//	double *B=(double *)b->get_memory();
	double *B = (double *)STARPU_MATRIX_GET_PTR(t->gpuArgs[1]);
	int ldB = b->get_rows();
	int N = b->get_cols();
	//cublasDtrsm ( side,  uplo,  transa, diag, m,  n,  alpha,A,  lda, B, ldb);
	cublasDtrsm('L','L','N','U',M,N,1.0,A,ldA,B,ldB);
	
	Dispatcher::finished(t);
      }
      
    };
    /*===================================================================================*/
    extern Trsm trsm_instance;
    extern unsigned int trsm_task_count ;
    /*===================================================================================*/
  template <typename P>
  class TrsmTask :public  Task<Trsm,P> {
        public:
            static const int trsm_type_id=1;
    TrsmTask(GData &A, GData &B, P* p):Task<Trsm,P>(&trsm_instance){
                this->args = new Args;
                this->axs  = new Axs;
                packArgs (  this->args, A , B    );
                packAxs  ( *this->axs , In, InOut);
                Task<Trsm,P>::id = trsm_task_count ++;
		Task<Trsm,P>::type_id = trsm_type_id;
		Task<Trsm,P>::child_count=0; 
		Task<Trsm,P>::set_parent(p);
            }
            ~TrsmTask(){
                delete this->args;
                delete this->axs;
            }
    };
    /*===================================================================================*/
    void utrsm(GData &A, GData &B);
    /*===================================================================================*/
    template <typename Scheduler,typename P>
    void Trsm::split(Scheduler &s,Task<Trsm,P> *task){
#     if UTP_DEBUG != 0
        cout << s.name <<"\tTrsm.split\t" << task->o->name <<"_" << task->id << endl;
#     endif
	GData &A =  *task->args->args[0];
	GData &B =  *task->args->args[1];
	GData &C =  *task->args->args[2];
        int xa = A.get_part_countX();
        int ya = A.get_part_countY();
        int yb = B.get_part_countY();
        for(int i=0;i<xa;i++){
	  for(int j=0;j<yb;j++){
	    TrsmTask<Task<Trsm,P>> *t = new TrsmTask<Task<Trsm,P>>(A(i,i),B(j,i),task);
	    Dispatcher::submit(s,t);
	    for ( int k=0;k<i;k++){
	      GemmTask<Task<Trsm,P>> *g = new GemmTask<Task<Trsm,P>>(A(i,k),B(j,k),B(j,i),task);
	      Dispatcher::submit(s,g);
	    }
	  }
        }
    }
    /*===================================================================================*/
 }// ublas namespace
}// utp namespace
#endif // UTRSM_HPP
