#ifndef UTRSM_HPP
#define UTRSM_HPP
#include "operation.hpp"
#include "ugemm.hpp"
#include "utp_basic.hpp"
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
            cout << "----\tTrsm.run\ttrsm_"<< t->id << endl;
	    GData *a = t->args->args[0];
	    GData *b = t->args->args[1];
	    printf("trsm------------, B(%d) \n",b->get_child_index());


	    double *A=(double *)a->get_memory();
	    int ldA = a->get_rows();

	    double *B=(double *)b->get_memory();
	    int ldB = b->get_rows();
	    int M = b->get_rows();
	    int N = b->get_cols();

	    // Signature: cblas_dtrsm (Order, Side, Uplo, TransA, Diag, M, N, alpha, A, lda, B, ldb)
	    printf("trsm------------, B(%d) \n",b->get_child_index());
	    cblas_dtrsm(CblasColMajor,CblasRight,CblasLower,CblasTrans,CblasNonUnit,M,N,1.0,A,ldA,B,ldB);
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
    TrsmTask(GData &A, GData &B):Task<Trsm,P>(&trsm_instance){
                Task<Trsm,P>::args = new Args;
                Task<Trsm,P>::axs  = new Axs;
                packArgs(Task<Trsm,P>::args,A,B);
                packAxs( *Task<Trsm,P>::axs, In, InOut);
                Task<Trsm,P>::id = trsm_task_count ++;
            }
            ~TrsmTask(){
                delete Task<Trsm,P>::args;
                delete Task<Trsm,P>::axs;
            }
    };
    /*===================================================================================*/
    void utrsm(GData &A, GData &B);
    /*===================================================================================*/
    template <typename Scheduler,typename P>
    void Trsm::split(Scheduler &s,Task<Trsm,P> *t){
        cout << s.name <<"\tTrsm.split\t" << t->o->name <<"_" << t->id << endl;
    }
    /*===================================================================================*/
 }// ublas namespace
}// utp namespace
#endif // UTRSM_HPP
