#include "ugemm.hpp"
#include <vector>
namespace utp{
    namespace ublas{
        Gemm gemm_instance("gemm");
      template<typename S,typename Oper,typename P>
      void ugemm(S &s,GData &A,GData &B, GData &C,Task<Oper,P> *p=nullptr){
            int m = A.get_part_countX();
            int n = B.get_part_countX();
            int o = C.get_part_countX();
            for ( int i=0;i<m;i++){
                for (int j=0;j<n;j++){
                    for (int k=0;k<o;k++){
		      GemmTask<Task<Oper,P>> *t = new GemmTask<Task<Oper,P>>(A(i,k),B(k,j),C(i,j),p);
                        Dispatcher::submit(s,t);
                    }
                }
            }
        }
      
        void ugemm(GData &A,GData &B, GData &C){
	  ugemm<UserProgram,Gemm,RootTask>(prog,A,B,C, static_cast<Task<Gemm,RootTask>*>(nullptr));
        }
      unsigned int gemm_task_count  = 0;

    } // ugemm namespace
} // utp namespace
