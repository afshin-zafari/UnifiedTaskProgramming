#include "dispatcher.hpp"

namespace utp{
  string UserProgram::name = "MainProgram";
  pthread_mutex_t 	DispatcherBase::parent_task_counter_lock;
  pthread_mutexattr_t 	DispatcherBase::parent_task_mutex_attr;  

}
