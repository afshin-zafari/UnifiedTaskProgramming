#include "cfg_sg_blas.hpp"
#include "cmdline.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"

void test_SG_BLAS(){
    int N  = cmdLine.M;
    int B1 = cmdLine.Mb;
    int B2 = cmdLine.mb;
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


    cout << "-----------------\n";

    utp::ublas::utrsm(A,B);
}
