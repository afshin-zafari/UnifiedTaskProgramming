#include "ugemm.hpp"
#include <vector>
namespace utp{
    namespace ublas{
        Gemm gemm_instance("gemm");
      template<typename S,typename Oper>
      void ugemm(S &s,GData &A,GData &B, GData &C,Task<OperationBase<Oper>> *p=nullptr){
            int m = A.get_part_countX();
            int n = B.get_part_countX();
            int o = C.get_part_countX();
            for ( int i=0;i<m;i++){
                for (int j=0;j<n;j++){
                    for (int k=0;k<o;k++){
		      GemmTask *t = new GemmTask(A(i,k),B(k,j),C(i,j),p);
                        Dispatcher::submit(s,t);
                    }
                }
            }
        }
        void ugemm(GData &A,GData &B, GData &C){
	  ugemm<UserProgram,Gemm>(prog,A,B,C);
        }
        unsigned int GemmTask::task_count  = 0;

    } // ugemm namespace
} // utp namespace
