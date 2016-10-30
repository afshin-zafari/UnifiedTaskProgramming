#ifndef UBLAS_HPP
#define UBLAS_HPP
#include "ugemm.hpp"
#include "utrsm.hpp"
namespace utp{
    template <typename S>
    void ready(S &s,unsigned int id, unsigned int type);
//    {
//        switch(type){
//        case ublas::GemmTask::gemm_type_id:
//            cout << 0 << endl;
//            //Dispatcher::ready(s,ublas::gemm_tasks[id]);
//            break;
//        case TRSM_TYPE_ID:
//            cout << 0 << endl;
//            break;
//
//        }
//    }
}//namespace utp
#endif // UBLAS_HPP
