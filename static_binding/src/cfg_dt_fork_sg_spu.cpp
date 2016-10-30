#include "cfg_dt_fork_sg_spu.hpp"

#include "ugemm.hpp"
#include "utrsm.hpp"

void test_DT_FORK_SG_SPU(){
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

    //utp::DT::ready(utp::ublas::t1); // simulate task ready in DT
    //utp::SG::ready(utp::ublas::t2); // simulate task ready in SG

    cout << "---------------------\n";

    utp::ublas::utrsm(A,B);
    //utp::DT::ready(utp::ublas::tr1); // simulate task ready in DT
    //utp::SPU::ready(utp::ublas::tr2); // simulate task ready in SPU

}

