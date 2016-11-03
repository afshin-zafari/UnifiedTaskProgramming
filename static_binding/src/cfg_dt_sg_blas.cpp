#include "cfg_dt_sg_blas.hpp"
#include "config.hpp"

#include "ugemm.hpp"
#include "utrsm.hpp"

void test_DT_SG_BLAS(){
    int N  = config.M;
    int B1 = config.Mb;
    int B2 = config.mb;
    utp::GData A(N,N,"A");
    utp::GData B(N,N,"B");
    utp::GData C(N,N,"C");
    utp::GPartitioner P1(B1,B1);
    utp::GPartitioner P2(B2,B2);
    P1.set_next(&P2);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);
    utp::ublas::ugemm(A,B,C);


    cout << "-----------------------------\n";
    utp::ublas::utrsm(A,B);

}
