#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP
#include <iostream>
#include "operation.hpp"
#include "GData.hpp"

using namespace std;
namespace utp{
class UserProgram{};
extern UserProgram prog;

/*-----------SingleNode ----------------------------------*/
template < typename T >
class SingleNode{
public:
    typedef T First;

};
template < typename E >
class NodeDispatch{
public:
    typedef typename SingleNode<typename E::First>::First first;
    template <typename T>
    static   void submit(UserProgram &, Task<OperationBase<T>>*t){
        cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E::First::submit(t);
    }
    template <typename T>
    static   void submit( Task<OperationBase<T>>*t){
        cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E::First::submit(t);
    }
    template <typename T>
    static   void ready(first &f,Task<OperationBase<T>>*t){
        cout <<  f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id << endl;
        t->o->split(f,t);
    }
    template <typename T>
    static   void finished(first &f,Task<OperationBase<T>>*t){
        cout <<  f.name <<"\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
    }

};

/*-----------Edge ----------------------------------*/
template < typename T, typename U >
class Edge{
public:
    typedef T First;
    typedef U Second;
};
/*----------Edge Dispatch---------------------------*/

template < typename E >
class EdgeDispatch{
public:
    typedef typename Edge<typename E::First, typename E::Second>::First  first;
    typedef typename Edge<typename E::First, typename E::Second>::Second second;
    template <typename T>
    static   void submit(UserProgram &, Task<OperationBase<T>>*t){
        cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E::First::submit(t);
    }
    template <typename T>
    static   void submit( Task<OperationBase<T>>*t){
        cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E::First::submit(t);
    }
    template <typename T>
    static   void submit(first &f, Task<OperationBase<T>>*t){
        cout << f.name << "\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E::Second::submit(t);
    }
    template <typename T>
    static   void ready(first &f,Task<OperationBase<T>>*t){
        cout <<  f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id << endl;
        t->o->run(t);
    }
    template <typename T>
    static   void finished(first &s,Task<OperationBase<T>>*t){}
  
    template <typename T>
    static   void finished(Task<OperationBase<T>>*t){
      E::First::finished(t);
    }

    template <typename T>
    static   void finished(second &s,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                E::First::finished(t->get_parent());
            }
        }
    }
};
/*-------Path Dispatch -----------------------------*/

template <typename E1, typename E2>
class PathDispatch{
public:
    typedef typename Edge<typename E1::First, typename E1::Second>::First  first;
    typedef typename Edge<typename E1::First, typename E1::Second>::Second second;
    typedef typename Edge<typename E2::First, typename E2::Second>::Second third;

    template <typename T>
    static   void submit(UserProgram &, Task<OperationBase<T>>*t){
        cout << "PROG\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        E1::First::submit(t);
    }
    template <typename T>
    static   void submit( Task<OperationBase<T>>*t){
        E1::First::submit(t);
    }
    template <typename T>
    static   void submit(first &, Task<OperationBase<T>>*t){
        E1::Second::submit(t);
    }
    template <typename T>
    static   void submit(second&, Task<OperationBase<T>>*t){
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count++;
	}
        E2::Second::submit(t);
    }
    template <typename T>
    static   void ready(first &f,Task<OperationBase<T>>*t){
        cout << f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id <<endl;
        t->o->split(f,t);
    }
    template <typename T>
    static   void ready(second &s,Task<OperationBase<T>>*t){
        cout << s.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id <<endl;
        t->o->run(t);
    }
    template <typename T>
    static   void finished(first &s,Task<OperationBase<T>>*t){}
    template <typename T>
    static   void finished(Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
	E1::Second::finished(t);
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                E1::First::finished(t->get_parent());
            }
        }
    }
    template <typename T>
    static   void finished(second &s,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                E1::First::finished(t->get_parent());
            }
        }
    }
    template <typename T>
    static   void finished(third &s,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                E1::Second::finished(t->get_parent());
            }
        }
    }
};
/*--------Fork -------------------------------------*/
template < typename F, typename T1, typename T2>
class Fork{
public :
    typedef F First;
    typedef T1 Second;
    typedef T2 Third;
};
/*--------Fork Dispatch-----------------------------*/ // 1st -> {2nd,3rd}
//typedef template < typename F, typename T1, typename T2> Fork::Second Edge1;

template < typename F>
class ForkDispatch{
public:
    typedef typename Fork<typename F::First, typename F::Second,typename F::Third>::First first;    //
    typedef typename Fork<typename F::First, typename F::Second,typename F::Third>::Second Edge1;   //              ---Edge1------
    typedef typename Fork<typename F::First, typename F::Second,typename F::Third>::Third  Edge2;   //           --> 2nd   --> 4th
    typedef typename Edge1::First second;                                                           // 1st -->  |
    typedef typename Edge2::First third;                                                            //           --> 3rd   --> 5th
    typedef typename Edge1::Second forth;                                                           //              ---Edge2------
    typedef typename Edge2::Second fifth;

    template <typename T>
    static   void submit(UserProgram &, Task<OperationBase<T>>*t){
        cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        F::First::submit(t);
    }
    template <typename T>
    static   void submit( Task<OperationBase<T>>*t){
        cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
        F::First::submit(t);
    }
    template <typename T>
    static   void submit(first &, Task<OperationBase<T>>*t){
        if ( !F::Second::First::submit(t) ) // if rejected by 2nd scheduler, forward it to 3rd
            F::Third::First::submit(t);
    }
    template <typename T>
    static   void submit(second&s, Task<OperationBase<T>>*t){
        F::Second::Second::submit(t);
    }
    template <typename T>
    static   void submit(third&, Task<OperationBase<T>>*t){
        F::Third::Second::submit(t);
    }
    template <typename T>
    static   void ready(first &f,Task<OperationBase<T>>*t){
        cout << f.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
        t->o->split(f,t);
    }
    template <typename T>
    static   void ready(second &s,Task<OperationBase<T>>*t){
        cout << s.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
        t->o->split(s,t);
    }
    template <typename T>
    static   void ready(third &trd,Task<OperationBase<T>>*t){
        cout << trd.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
        t->o->split(trd,t);
    }
    template <typename T>
    static   void finished(first &s,Task<OperationBase<T>>*t){}

    template <typename T>
    static   void finished(second &s,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                F::First::finished(t->get_parent());
            }
        }
    }
    template <typename T>
    static   void finished(third &s,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                F::First::finished(t->get_parent());
            }
        }
    }
    template <typename T>
    static   void finished(forth &frth,Task<OperationBase<T>>*t){
        cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
        if ( t->get_parent() != nullptr){
            t->get_parent()->child_count--;
            if(t->get_parent()->child_count == 0 ){
                F::Second::First::finished(t->get_parent());
            }
        }
    }


};
class BLAS;
class SG;
class DT;
class DT2;
class SPU;
class cuBLAS;
#ifdef CUBLAS_ONLY
typedef NodeDispatch<SingleNode<cuBLAS>> Dispatcher;
#endif // CUBLAS_ONLY

#ifdef BLAS_ONLY
typedef NodeDispatch<SingleNode<BLAS>> Dispatcher;
#endif // BLAS_ONLY
#ifdef SG_BLAS
typedef EdgeDispatch<Edge<SG,BLAS>> Dispatcher;
#endif // SG_BLAS
#ifdef DT_BLAS
typedef EdgeDispatch<Edge<DT,BLAS>> Dispatcher;
#endif // DT_BLAS
#ifdef DT_SG_BLAS
typedef Edge<DT,SG>   DT_SG_Edge;
typedef Edge<SG,BLAS> SG_BLAS_Edge;
typedef PathDispatch<DT_SG_Edge,SG_BLAS_Edge> Dispatcher;
#endif // DT_SG_BLAS
#ifdef DT_FORK_SG_SPU
typedef Edge<SG ,BLAS>   SG_BLAS_Edge;
typedef Edge<SPU,BLAS>  SPU_BLAS_Edge;
typedef ForkDispatch<Fork<DT, SG_BLAS_Edge,SPU_BLAS_Edge>> Dispatcher;
typedef Edge<DT2,BLAS>  DT2_BLAS_Edge;
typedef ForkDispatch<Fork<DT, DT2_BLAS_Edge,SG_BLAS_Edge>> Dispatcher;
#endif // DT_FORK_SG_SPU
} // namespace utp
#endif // DISPATCHER_HPP
