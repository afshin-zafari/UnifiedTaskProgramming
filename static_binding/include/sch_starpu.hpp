#ifndef SCH_STARPU_HPP
#define SCH_STARPU_HPP
#include "operation.hpp"
#include "dispatcher.hpp"

namespace utp{
    template <typename T> class OperationBase;
    template <typename T> class Task;
    class SPU;
    extern SPU _spu;

class SPU{
public:
    string name ;
    SPU():name("SPU"){}
    template <typename T>
    static inline void ready(Task<OperationBase<T>> *t){
        cout << "----\t SPU.ready\t" << t->o->name << "_" << t->id << endl;
        utp::Dispatcher::ready(_spu,t);
    }
    template<typename T>
    static inline int submit(Task<OperationBase<T>>*t){
        cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl;
        if ( t->id %2==1)
            return 0;
        cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl;
        return 1;
    }
    template <typename T>
    static inline void finished(Task<OperationBase<T>> *t){
        std::cout << "----\t SPU.finished\t" << t->o->name << "_" << t->id << endl;
        utp::Dispatcher::finished(_spu,t);
    }

};
extern SPU _spu;
}
#endif // SCH_STARPU_HPP
