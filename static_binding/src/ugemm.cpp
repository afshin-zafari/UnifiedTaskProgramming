#include "ugemm.hpp"
#include <vector>
namespace utp{
    namespace ublas{
        Gemm gemm_instance("gemm");
      
        void ugemm(GData &A,GData &B, GData &C){
	  GemmTask<Task<Gemm,RootTask>> *t = new GemmTask<Task<Gemm,RootTask>>(A,B,C, static_cast<Task<Gemm,RootTask>*>(nullptr));
	  Gemm::split(prog,t);
        }
      unsigned int gemm_task_count  = 0;

    } // ugemm namespace
} // utp namespace
