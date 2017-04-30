#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP
#include <iostream>
#include "operation.hpp"
#include "GData.hpp"

using namespace std;
namespace utp{
  class UserProgram{
  public:
    static string name;
  };
  extern UserProgram prog;

  /*-----------SingleNode ----------------------------------*/
  template < typename T >
  class SingleNode{
  public:
    typedef T First;

  };
  /*-----------Node Dispatch ----------------------------------*/
  template < typename E >
  class NodeDispatch{
  public:
    typedef typename SingleNode<typename E::First>::First first;
  
    static bool is_distributed(int level){return false;}
  
    template <typename T,typename P>
    static   void submit(UserProgram &, Task<T,P>*t){
#     if DEBUG!=0
      cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      E::First::submit(t);
    }
    template <typename T,typename P>
    static   void submit( Task<T,P>*t){
#     if DEBUG!=0
      cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
	
      E::First::submit(t);
    }
    template <typename T,typename P>
    static   void ready(first &f,Task<T,P>*t){
#     if DEBUG!=0
      cout <<  f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      t->o->split(f,t);
    }
    template <typename T,typename P>
    static   void finished(Task<T,P>*t){
#     if DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
    }
    template <typename T,typename P>
    static   void finished(first &f,Task<T,P>*t){
#     if DEBUG!=0
      cout << f.name << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
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
    /*-------------------------------------------------------*/
    static bool is_distributed(int level){
      if ( level ==1 && first::name == "DT" )
	return true;
      return false;
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static void submit(UserProgram &, Task<T,P>*t){
#     if DEBUG!=0
      cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      E::First::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static void submit( Task<T,P>*t){
#     if DEBUG!=0
      cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      E::First::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void submit(first &f, Task<T,P>*t){
#     if DEBUG!=0
      cout << f.name << "\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      E::Second::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void ready(first &f,Task<T,P>*t){
#     if DEBUG!=0
      cout <<  f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      t->o->run(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static void ready_for_gpu(first &f,Task<T,P>*t){
      #if UTP_DEBUG != 0
      cout <<  f.name <<"\t Dis.ready_for_gpu\t" <<  t->o->name << "_" << t->id << endl;
      #endif
      t->o->run_on_gpu(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void finished(first &s,Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif

    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void finished(Task<T,P>*t){
      E::First::finished(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void finished(second &s,Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      if ( t->get_parent() != nullptr){
	Atomic::decrease(&t->get_parent()->child_count);
	if(t->get_parent()->child_count == 0 ){
	  E::First::finished(t->get_parent());
	}
      }
    }
  };


  /*======== -------Path Dispatch -------================*/

  template <typename E1, typename E2>
  class PathDispatch{
  public:
    typedef typename Edge<typename E1::First, typename E1::Second>::First  first;
    typedef typename Edge<typename E1::First, typename E1::Second>::Second second;
    typedef typename Edge<typename E2::First, typename E2::Second>::Second third;

    /*-------------------------------------------------------*/
    static void Init(){
      first::level = 0;
      second::level = 1;
      //third::level = 2;
    }
    /*-------------------------------------------------------*/
    static bool is_distributed(int level){
      switch(level){
      case 0:
	if ( first::name == "DT")
	  return true;
	return false;
      case 1:
	if ( first::name == "DT")
	  return true;
	return false;
      case 2:
	if ( second::name == "DT")
	  return true;
	return false;
      case 3:
	return false;
      }
      return false;
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void submit(UserProgram &, Task<T,P>*t){
#     if DEBUG!=0
      cout << "PROG\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      E1::First::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void submit( Task<T,P>*t){
      E1::First::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void submit(first &, Task<T,P>*t){
      E1::Second::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void submit(second&, Task<T,P>*t){
      E2::Second::submit(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static void ready_for_gpu(second  &s,Task<T,P>*t){
      #if UTP_DEBUG !=0
      cout <<  s.name <<"\t Dis.ready_for_gpu\t" <<  t->o->name << "_" << t->id << endl;
      #endif
      t->o->run_on_gpu(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void ready(first &f,Task<T,P>*t){
#     if UTP_DEBUG != 0
      cout << f.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id <<endl;
#     endif
      t->o->split(f,t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void ready(second &s,Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << s.name <<"\t Dis.ready\t" <<  t->o->name << "_" << t->id <<endl;
#     endif
      t->o->run(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   void finished(first &s,Task<T,P>*t){}
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static  inline  void finished(Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << "+++++++\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      E1::Second::finished(t);
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline void finished(second &s,Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << s.name << " Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      P *p = t->get_parent();
      if ( p != nullptr){
	unsigned int new_count=1;
	new_count = Atomic::decrease_nv(&p->child_count);
	bool finished = new_count == 0;
#     if UTP_DEBUG!=0
	cout << "ppppp Dis.finished\t" <<  p->o->name << "_" << p->id << "child Count" << new_count << endl;
#     endif
	if( finished){
#     if UTP_DEBUG!=0
	  cout << "ppppp Dis.finished\t" <<  p->o->name << "_" << p->id << endl;
#     endif
	  E1::First::finished(p);
	}
      }
    }
    /*-------------------------------------------------------*/
    template <typename T,typename P>
    static   inline  void finished(third &s,Task<T,P>*t){
#     if UTP_DEBUG!=0
      cout << s.name << " Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      P *p = t->get_parent();
      if ( p != nullptr){
	unsigned int new_count=1;
	new_count = Atomic::decrease_nv(&p->child_count);
	bool finished = new_count == 0;
	if(finished){
	  E1::Second::finished(p);
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
    static bool is_distributed(int level){
      if ( level ==1)
	return true;
      return false;
    }
    template <typename T,typename P>
    static   void submit(UserProgram &, Task<T,P>*t){
#     if DEBUG!=0
      cout << "Prog\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      F::First::submit(t);
    }
    template <typename T,typename P>
    static   void submit( Task<T,P>*t){
#     if DEBUG!=0
      cout << "----\t Dis.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
      F::First::submit(t);
    }
    template <typename T,typename P>
    static   void submit(first &, Task<T,P>*t){
      if ( !F::Second::First::submit(t) ) // if rejected by 2nd scheduler, forward it to 3rd
	F::Third::First::submit(t);
    }
    template <typename T,typename P>
    static   void submit(second&s, Task<T,P>*t){
      F::Second::Second::submit(t);
    }
    template <typename T,typename P>
    static   void submit(third&, Task<T,P>*t){
      F::Third::Second::submit(t);
    }
    template <typename T,typename P>
    static   void ready(first &f,Task<T,P>*t){
#     if DEBUG!=0
      cout << f.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
      t->o->split(f,t);
    }
    template <typename T,typename P>
    static   void ready(second &s,Task<T,P>*t){
#     if DEBUG!=0
      cout << s.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
      t->o->split(s,t);
    }
    template <typename T,typename P>
    static   void ready(third &trd,Task<T,P>*t){
#     if DEBUG!=0
      cout << trd.name << "\t Dis.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
      t->o->split(trd,t);
    }
    template <typename T,typename P>
    static   void finished(first &s,Task<T,P>*t){}

    template <typename T,typename P>
    static   void finished(second &s,Task<T,P>*t){
#     if DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      if ( t->get_parent() != nullptr){
	Atomic::decrease(&t->get_parent()->child_count);
	if(t->get_parent()->child_count == 0 ){
	  F::First::finished(t->get_parent());
	}
      }
    }
    template <typename T,typename P>
    static   void finished(third &s,Task<T,P>*t){
#     if DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      if ( t->get_parent() != nullptr){
	Atomic::decrease(&t->get_parent()->child_count);
	if(t->get_parent()->child_count == 0 ){
	  F::First::finished(t->get_parent());
	}
      }
    }
    template <typename T,typename P>
    static   void finished(forth &frth,Task<T,P>*t){
#     if DEBUG!=0
      cout << "\t Dis.finished\t" <<  t->o->name << "_" << t->id << endl;
#     endif
      if ( t->get_parent() != nullptr){
	Atomic::decrease(&t->get_parent()->child_count);
	if(t->get_parent()->child_count == 0 ){
	  F::Second::First::finished(t->get_parent());
	}
      }
    }


  };
  class BLAS;
  class SPU;
  class SG;
  class DT;
  class DT2;
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

#ifdef SPU_BLAS
  typedef EdgeDispatch<Edge<SPU,BLAS>> Dispatcher;
#endif //SPU_BLAS

#ifdef DT_SPU_BLAS
  typedef Edge<DT,SPU>   DT_SPU_Edge;
  typedef Edge<SPU,BLAS> SPU_BLAS_Edge;
  typedef PathDispatch<DT_SPU_Edge,SPU_BLAS_Edge> Dispatcher;
#endif // DT_SPU_BLAS

  
#ifdef DT_FORK_SG_SPU
  typedef Edge<SG ,BLAS>   SG_BLAS_Edge;
  typedef Edge<SPU,BLAS>  SPU_BLAS_Edge;
  typedef ForkDispatch<Fork<DT, SG_BLAS_Edge,SPU_BLAS_Edge>> Dispatcher;
  typedef Edge<DT2,BLAS>  DT2_BLAS_Edge;
  typedef ForkDispatch<Fork<DT, DT2_BLAS_Edge,SG_BLAS_Edge>> Dispatcher;
#endif // DT_FORK_SG_SPU
} // namespace utp
#endif // DISPATCHER_HPP
