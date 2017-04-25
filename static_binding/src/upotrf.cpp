#include "upotrf.hpp"

namespace utp{
    namespace ublas{
        Potrf potrf_instance("potrf");
      
        void upotrf(GData &A){
	  PotrfTask<Task<Potrf,RootTask>> *t = new PotrfTask<Task<Potrf,RootTask>>(A, static_cast<Task<Potrf,RootTask>*>(nullptr));
	  Potrf::split(prog,t);
        }
      unsigned int potrf_task_count  = 0;

    } // upotrf namespace
} // utp namespace
