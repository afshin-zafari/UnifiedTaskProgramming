#ifndef OPERATION_HPP
#define OPERATION_HPP
#include <iostream>
#include "utp_basic.hpp"
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
    unsigned int id,child_count;
    Task ( OperationBase<T> *_o):o(_o){
        child_count =0;
        parent = nullptr;
    }
    void set_parent(Parent  *p){
        parent = p;
	if ( p ) 
	  p->child_count ++; // ToDo: has to be Atomic ++
    }
    Parent *get_parent(){return parent;}
private:
    Parent *parent;
};


class Base{};
template<typename T>
class OperationBase: public ISplit<T> ,    public Operation<Base> {};
} // utp namespace
#endif // OPERATION_HPP
