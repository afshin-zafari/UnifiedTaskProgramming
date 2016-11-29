#ifndef OPERATION_HPP
#define OPERATION_HPP
#include <iostream>
#include "utp_basic.hpp"
#include "atomic.hpp"

using namespace sg;
namespace utp{



template <typename S>
class Operation{
public:
    std::string name;
    Operation(){}
    Operation(const char *n):name(n){}
};

  template<typename T,typename P> class Task;
  template<typename T> class OperationBase;


template <typename T>
class ISplit{
  public:
      template <typename O,typename P>
      Task<O,P>* getHost(){
        static_cast<T*>(this)->getHost();
      }
    template<typename S,typename O,typename P>
    void split(S&s,Task<O,P>*t){
        static_cast<T*>(this)->split(s,t);
    }
    template<typename O,typename P>
    void run(Task<O,P>*t){
        static_cast<T*>(this)->run(t);
    }
};

  class RootTask{
  public :
    unsigned int child_count ;
  };
  template<typename T,typename Parent>
class Task{
public:
    Args *args;
    Axs *axs;
    void *guest;
    int type_id;
    OperationBase<T> *o;
    int id,child_count;
    bool is_generating;
    Task ( OperationBase<T> *_o):o(_o){
        child_count =0;
        parent = nullptr;
	Init();
    }
    void set_parent(Parent  *p){
        parent = p;
	if ( p ) 
	  Atomic::increase(&p->child_count) ;
    }
    Parent *get_parent(){return parent;}
     void Init(){
       /*
      pthread_mutexattr_init   (&parent_task_mutex_attr);
      pthread_mutexattr_settype(&parent_task_mutex_attr,PTHREAD_MUTEX_RECURSIVE);
      pthread_mutex_init       (&parent_task_counter_lock,&parent_task_mutex_attr);
       */
    }
     void BeginCriticalSection_obsolete(){ pthread_mutex_lock  (&parent_task_counter_lock); }
     void   EndCriticalSection_obsolete(){ pthread_mutex_unlock(&parent_task_counter_lock); }
private:
    pthread_mutex_t 	parent_task_counter_lock;
    pthread_mutexattr_t parent_task_mutex_attr;  
    Parent *parent;
};


class Base{};
template<typename T>
class OperationBase: public ISplit<T> ,    public Operation<Base> {};
} // utp namespace
#endif // OPERATION_HPP
