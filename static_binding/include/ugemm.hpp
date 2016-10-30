#ifndef UGEMM_HPP
#define UGEMM_HPP
#include "utp.hpp"
namespace utp{
    namespace ublas{
    /*===================================================================================*/
    class Gemm: public OperationBase<Gemm>{
    public:
        Gemm(string n){this->name = n;};
        string get_name(){return name;}
        template <typename Scheduler>
        static inline void split(Scheduler &s,Task<OperationBase<Gemm>> *t);
        static inline void run(Task<OperationBase<Gemm>> *t){
            cout << "----\tGemm.run\tgemm_"<< t->id << endl;
        }
    };
    /*===================================================================================*/
    extern Gemm gemm_instance;
    /*===================================================================================*/
    class GemmTask: public Task<OperationBase<Gemm>>{
    public:
        static unsigned int task_count;
        static const int gemm_type_id=0;
        Task<OperationBase<Gemm>>*getHost(){
            return this;
        }
        GemmTask(GData &A,GData &B,GData &C):Task<OperationBase<Gemm>>(&gemm_instance){
            args = new Args;
            axs = new Axs;
            packArgs(args, A , B  , C    );
            packAxs (*axs, In, In , InOut);
            id = GemmTask::task_count ++;
            type_id = gemm_type_id;
        }
        ~GemmTask(){
            delete args;
            delete axs;
        }
    };
    /*===================================================================================*/
    static vector<GemmTask *> gemm_tasks;
    /*===================================================================================*/
    void ugemm(GData &,GData &, GData &);
    template<typename S>
    void ugemm(S &s,GData &A,GData &B, GData &C);

    /*===================================================================================*/
    template <typename Scheduler>
    void Gemm::split(Scheduler &s,Task<OperationBase<Gemm>> *t){
        cout << s.name <<"\tGemm.split\t" << t->o->name <<"_" << t->id << endl;
        ugemm<Scheduler>(s,*t->args->args[0],*t->args->args[1],*t->args->args[2]);
    }
 } // ublas namespace
} // utp namespace
#endif // UGEMM_HPP
