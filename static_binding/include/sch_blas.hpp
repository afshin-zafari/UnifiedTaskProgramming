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
  template <typename T,typename P>
  static inline int submit(Task<T,P> *t){
        std::cout << "----\tBLAS.submit\t" << t->o->name << "_" << t->id <<endl;
        t->o->run(t);
        Dispatcher::finished(_blas,t);
        return 1;
    }
    template <typename T,typename P>
    static inline void run(Task<T,P> *t){
        std::cout << "run task " << t->o->name << " in BLAS.\n";
    }
  template <typename T,typename P>
  static inline void finished(Task<T,P> *t){
        std::cout << "----\tBLAS.finished\t" << t->o->name << "_" << t->id << endl;
    }
};
} // BLAS

#endif // SCH_BLAS_HPP
