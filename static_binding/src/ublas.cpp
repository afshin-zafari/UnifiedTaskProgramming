#include "ublas.hpp"
namespace utp{
    template <typename S>
    void ready(S &s,unsigned int id, unsigned int type)
    {
        switch(type){
        case ublas::GemmTask::gemm_type_id:
            cout << 0 << endl;
            Dispatcher::ready(s,ublas::gemm_tasks[id]);
            break;
        case ublas::TrsmTask::trsm_type_id:
            cout << 0 << endl;
            break;

        }
    }
    template <>
    void ready<SG>(SG &s,unsigned int id, unsigned int type)
    {
        switch(type){
        case ublas::GemmTask::gemm_type_id:
            cout << 0 << endl;
            //Dispatcher::ready(s,ublas::gemm_tasks[id]);
            break;
        case ublas::TrsmTask::trsm_type_id:
            cout << 0 << endl;
            break;

        }
    }
}//namespace utp

