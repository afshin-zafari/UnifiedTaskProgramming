#include "utrsm.hpp"

namespace utp{
namespace ublas{
    /*========================================*/
    Trsm trsm_instance("trsm");
    /*========================================*/
    unsigned int TrsmTask::task_count =0;

    /*========================================*/
    template<typename P>
    void utrsm(P &p,GData &A, GData &B){
        int xa = A.get_part_countX();
        int ya = A.get_part_countY();
        int yb = B.get_part_countY();
        for(int i=0;i<xa;i++){
            for(int j=0;j<yb;j++){
                TrsmTask *t = new TrsmTask(A(i,i),B(j,i));
                Dispatcher::submit(p,t);
                for ( int k=i+1;k<ya;k++){
                    for (int l=0;l<xa;l++){
                        GemmTask *g = new GemmTask(A(k,i),B(j,i),B(j,l));
                        Dispatcher::submit(p,g);
                    }
                }
            }
        }
    }
    void utrsm(GData &A, GData &B){
        utrsm<UserProgram>(prog,A,B);
    }
    /*========================================*/
 } // namespace ublas
}//namespace utp
