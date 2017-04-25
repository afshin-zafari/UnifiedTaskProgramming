#include "ugemm.hpp"
#include "usyrk.hpp"

namespace utp{
    namespace ublas{
        Syrk syrk_instance("syrk");
      
        void usyrk(GData &A,GData &B){
	  SyrkTask<Task<Syrk,RootTask>> *t = new SyrkTask<Task<Syrk,RootTask>>(A,B, static_cast<Task<Syrk,RootTask>*>(nullptr));
	  Syrk::split(prog,t);
        }
      unsigned int syrk_task_count  = 0;

    } // usyrk namespace
} // utp namespace
