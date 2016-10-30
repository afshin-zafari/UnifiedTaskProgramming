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
        template <typename Scheduler>
        static inline void split(Scheduler &s,Task<OperationBase<Trsm>> *t);
        static inline void run(Task<OperationBase<Trsm>> *t){
            cout << "----\tTrsm.run\ttrsm_"<< t->id << endl;
        }
    };
    /*===================================================================================*/
    extern Trsm trsm_instance;
    /*===================================================================================*/
    class TrsmTask :public  Task<OperationBase<Trsm>> {
        public:
            static unsigned int task_count ;
            static const int trsm_type_id=1;
            TrsmTask(GData &A, GData &B):Task<OperationBase<Trsm>>(&trsm_instance){
                args = new Args;
                axs  = new Axs;
                packArgs(args,A,B);
                packAxs( *axs, In, InOut);
                id = TrsmTask::task_count ++;
            }
            ~TrsmTask(){
                delete args;
                delete axs;
            }
    };
    /*===================================================================================*/
    void utrsm(GData &A, GData &B);
    /*===================================================================================*/
    template <typename Scheduler>
    void Trsm::split(Scheduler &s,Task<OperationBase<Trsm>> *t){
        cout << s.name <<"\tTrsm.split\t" << t->o->name <<"_" << t->id << endl;
    }
    /*===================================================================================*/
 }// ublas namespace
}// utp namespace
#endif // UTRSM_HPP
