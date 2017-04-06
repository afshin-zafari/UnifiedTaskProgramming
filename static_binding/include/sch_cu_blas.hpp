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
    template <typename T,typename P>
    static inline int submit(Task<T,P> *t){
#     if DEBUG != 0 
      std::cout << "----\tCUDA.submit\t" << t->o->name << "_" << t->id <<endl;
#     endif
      t->o->run(t);
      Dispatcher::finished(_cublas,t);
      return 1;
    }
    template <typename T,typename P>
    static inline void run(Task<T,P> *t){
#     if DEBUG != 0 
      std::cout << "run task " << t->o->name << " in BLAS.\n";
#     endif
    }
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
#     if DEBUG != 0 
      std::cout << "----\tCUDA.finished\t" << t->o->name << "_" << t->id << endl;
#     endif
    }
  };
} //

#endif // SCH_CU_BLAS_HPP
