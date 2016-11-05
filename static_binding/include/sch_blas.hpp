#ifndef SCH_BLAS_HPP
#define SCH_BLAS_HPP

#include <string>
#include <iostream>
#include "utp.hpp"
using namespace std;
namespace utp{
    class BLAS;
    extern BLAS _blas;

class BLAS{
public:
    std::string name ;
    BLAS():name("BLAS"){}
    static void data_created(GData *){
    }
    template <typename T>
    static inline int submit(Task<OperationBase<T>> *t){
        std::cout << "----\tBLAS.submit\t" << t->o->name << "_" << t->id <<endl;
        t->o->run(t);
        Dispatcher::finished(_blas,t);
        return 1;
    }
    template <typename T>
    static inline void run(Task<OperationBase<T>> *t){
        std::cout << "run task " << t->o->name << " in BLAS.\n";
    }
    template <typename T>
    static inline void finished(Task<OperationBase<T>> *t){
        std::cout << "----\tBLAS.finished\t" << t->o->name << "_" << t->id << endl;
    }
};
} // BLAS

#endif // SCH_BLAS_HPP
