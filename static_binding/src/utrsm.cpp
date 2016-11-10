#include "utrsm.hpp"

namespace utp{
namespace ublas{
    /*========================================*/
    Trsm trsm_instance("trsm");
    /*========================================*/
    unsigned int trsm_task_count =0;

    /*========================================*/
  template<typename S, typename P>
  void utrsm(S &s,GData &A, GData &B, P *p=nullptr){
    /*
        int xa = A.get_part_countX();
        int ya = A.get_part_countY();
        int yb = B.get_part_countY();
        for(int i=0;i<xa;i++){
            for(int j=0;j<yb;j++){
	      TrsmTask<P> *t = new TrsmTask<P>(A(i,i),B(j,i));
                Dispatcher::submit(s,t);
                for ( int k=i+1;k<ya;k++){
                    for (int l=0;l<xa;l++){
		      GemmTask<P> *g = new GemmTask<P>(A(k,i),B(j,i),B(j,l),p);
                        Dispatcher::submit(s,g);
                    }
                }
            }
        }
	*/
    }
    void utrsm(GData &A, GData &B){
      //      utrsm<UserProgram,RootTask>(prog,A,B);
      TrsmTask<RootTask> *t = new TrsmTask<RootTask>(A,B, nullptr);
      Trsm::split(prog,t);
      
    }
    /*========================================*/
 } // namespace ublas
}//namespace utp
