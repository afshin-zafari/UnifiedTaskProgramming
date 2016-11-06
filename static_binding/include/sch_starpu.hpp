#ifndef SCH_STARPU_HPP
#define SCH_STARPU_HPP
#include "operation.hpp"
#include "dispatcher.hpp"

namespace utp{
    template <typename T> class OperationBase;
    template <typename T,typename P> class Task;
    class SPU;
    extern SPU _spu;

class SPU{
public:
    string name ;
    SPU():name("SPU"){}
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
        cout << "----\t SPU.ready\t" << t->o->name << "_" << t->id << endl;
        utp::Dispatcher::ready(_spu,t);
    }
    template<typename T,typename P>
    static inline int submit(Task<T,P>*t){
        cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl;
        if ( t->id %2==1)
            return 0;
        cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl;
        return 1;
    }
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
        std::cout << "----\t SPU.finished\t" << t->o->name << "_" << t->id << endl;
        utp::Dispatcher::finished(_spu,t);
    }

};
extern SPU _spu;
}
#endif // SCH_STARPU_HPP
