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
#         if DEBUG != 0
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
    TrsmTask(GData &A, GData &B, P* p):Task<Trsm,P>(&trsm_instance){
                Task<Trsm,P>::args = new Args;
                Task<Trsm,P>::axs  = new Axs;
                packArgs (  Task<Trsm,P>::args, A , B    );
                packAxs  ( *Task<Trsm,P>::axs , In, InOut);
                Task<Trsm,P>::id = trsm_task_count ++;
		Task<Trsm,P>::type_id = trsm_type_id;
		Task<Trsm,P>::child_count=0; 
		Task<Trsm,P>::set_parent(p);
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
    void Trsm::split(Scheduler &s,Task<Trsm,P> *task){
#     if DEBUG != 0
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
                for ( int k=i+1;k<ya;k++){
                    for (int l=0;l<xa;l++){
		      GemmTask<Task<Trsm,P>> *g = new GemmTask<Task<Trsm,P>>(A(k,i),B(j,i),B(j,l),task);
                        Dispatcher::submit(s,g);
                    }
                }
            }
        }
    }
    /*===================================================================================*/
 }// ublas namespace
}// utp namespace
#endif // UTRSM_HPP
