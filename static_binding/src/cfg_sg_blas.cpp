#include "cfg_sg_blas.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"

void test_SG_BLAS(){
    utp::SG_Engine = new SuperGlue<utp::SGWOptions>;
    const int N = 12;
    utp::GData A(N,N,"A");
    utp::GData B(N,N,"B");
    utp::GData C(N,N,"C");
    utp::GPartitioner P1(2,2);
    utp::GPartitioner P2(3,3);
    P1.set_next(&P2);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);

    utp::ublas::ugemm(A,B,C);

    //utp::SGTask<utp::ublas::Gemm>::tasks[0]->run(); // simulate task ready in SG

    cout << "-----------------\n";

    utp::ublas::utrsm(A,B);
//    utp::SG::ready(utp::ublas::t1); // simulate task ready in SG
    utp::SG_Engine->barrier();
}
