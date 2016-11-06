#ifndef UGEMM_HPP
#define UGEMM_HPP
#include "utp.hpp"
#include "gsl/gsl_cblas.h"

namespace utp{
    namespace ublas{
    /*===================================================================================*/
    class Gemm: public OperationBase<Gemm>{
    public:
        Gemm(string n){this->name = n;};
        string get_name(){return name;}
        template <typename Scheduler,typename P>
        static inline void split(Scheduler &s,Task<Gemm,P> *t);
      template <typename P>
      static inline void run(Task<Gemm,P> *t){
            cout << "----\tGemm.run\tgemm_"<< t->id << endl;
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
	    double beta= 1.0; 
	    double alpha= 1.0;
	    auto TransA = CblasNoTrans;
	    //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
	    bool dbg = false;

	    if (dbg) {
	      cout << "-----------\n" <<
		a->get_name() << "*"  <<
		b->get_name() << "->" <<
		c->get_name() << "\n---------------\n";	
	      cout << A << "," << B << "," << C << endl;
	      printf("%p,%p,%p\n%d,%d,%d\n%d,%d,%d\n",A,B,C,ldA,ldB,ldC,M,N,K);
	      cout << "===============================\n";
	      a->print();
	      cout << "===============================\n";
	      b->print();
	      cout << "===============================\n";
	    }
	    cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,M,N,K,alpha,A,ldA,B,ldB,beta,C,ldC);
	    if (dbg)
	      cout << "-----------\n" ;
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
        Task<Gemm,P>*getHost(){
            return this;
        }
	GemmTask(GData &A,GData &B,GData &C,P *p=nullptr):Task<Gemm,P>(&gemm_instance){
	  Task<Gemm,P>::args = new Args;
	  Task<Gemm,P>::axs = new Axs;
            packArgs( Task<Gemm,P>::args, A , B  , C    );
            packAxs (*Task<Gemm,P>::axs , In, In , InOut);
	    Task<Gemm,P>::id = gemm_task_count ++;
	    Task<Gemm,P>::type_id = gemm_type_id;
	    Task<Gemm,P>::child_count=0;
	    Task<Gemm,P>::set_parent(p);
        }
        ~GemmTask(){
            delete Task<Gemm,P>::args;
            delete Task<Gemm,P>::axs;
        }
    };
    /*===================================================================================*/
      void ugemm(GData &,GData &, GData &);
      template<typename S,typename Oper,typename P>
      void ugemm(S &s,GData &A,GData &B, GData &C,Task<Oper,P >* p=nullptr);

    /*===================================================================================*/
    template <typename Scheduler,typename P>
    void Gemm::split(Scheduler &s,Task<Gemm,P> *t){
        cout << s.name <<"\tGemm.split\t" << t->o->name <<"_" << t->id << endl;
        ugemm<Scheduler,Gemm>(s,*t->args->args[0],*t->args->args[1],*t->args->args[2],t);
    }
 } // ublas namespace
} // utp namespace
#endif // UGEMM_HPP
