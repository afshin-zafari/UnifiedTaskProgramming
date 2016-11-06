#ifndef SCH_CU_BLAS_HPP
#define SCH_CU_BLAS_HPP

#include <string>
#include <iostream>
#include "utp.hpp"
using namespace std;
namespace utp{
    class cuBLAS;
    extern cuBLAS _cublas;

class cuBLAS{
public:
    std::string name ;
    cuBLAS():name("CUDA"){}
    template <typename T>
    static inline int submit(Task<T> *t){
        std::cout << "----\tCUDA.submit\t" << t->o->name << "_" << t->id <<endl;
        t->o->run(t);
        Dispatcher::finished(_cublas,t);
        return 1;
    }
    template <typename T>
    static inline void run(Task<T> *t){
        std::cout << "run task " << t->o->name << " in BLAS.\n";
    }
    template <typename T>
    static inline void finished(Task<T> *t){
        std::cout << "----\tCUDA.finished\t" << t->o->name << "_" << t->id << endl;
    }
};
} //

#endif // SCH_CU_BLAS_HPP
