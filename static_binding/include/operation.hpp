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

template<typename T> class Task;
template<typename T> class OperationBase;;


template <typename T>
class ISplit{
  public:
      template <typename O>
      Task<OperationBase<O>>* getHost(){
        static_cast<T*>(this)->getHost();
      }
    template<typename S,typename O>
    void split(S&s,Task<OperationBase<O>>*t){
        static_cast<T*>(this)->split(s,t);
    }
    template<typename O>
    void run(Task<OperationBase<O>>*t){
        static_cast<T*>(this)->run(t);
    }
};

template<typename T>
class Task{
public:
    Args *args;
    Axs *axs;
    void *guest;
    int type_id;
    T *o;
    unsigned int id,child_count;
    Task ( T *_o):o(_o){
        child_count =0;
        parent = nullptr;
    }
    void set_parent(Task *p){
        parent = p;
        p->child_count ++; // ToDo: has to be Atomic ++
    }
    Task *get_parent(){return parent;}
private:
    Task *parent;
};


class Base{};
template<typename T>
class OperationBase: public ISplit<T> ,    public Operation<Base> {};
} // utp namespace
#endif // OPERATION_HPP
