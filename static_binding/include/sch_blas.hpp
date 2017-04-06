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
    static string name ;
    static int level;
    BLAS(){}
  
    static void data_created(GData *){
    }
    template <typename T,typename P>
    static inline int submit(Task<T,P> *t){
#     if DEBUG!=0
      std::cout << "----\tBLAS.submit\t" << t->o->name << "_" << t->id <<endl;
#     endif
      t->o->run(t);
      Dispatcher::finished(_blas,t);
      return 1;
    }
    template <typename T,typename P>
    static inline void run(Task<T,P> *t){
#     if DEBUG!=0
      std::cout << "run task " << t->o->name << " in BLAS.\n";
#     endif
    }
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
#     if DEBUG!=0
      std::cout << "----\tBLAS.finished\t" << t->o->name << "_" << t->id << endl;
#     endif
    }
  };
} // BLAS

#endif // SCH_BLAS_HPP
