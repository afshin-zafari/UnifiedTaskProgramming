#include "cfg_blas_only.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
void test_BLAS_ONLY(){
    utp::GData A(10,10,"A");
    utp::GData B(10,10,"B");
    utp::GData C(10,10,"C");
    utp::GPartitioner P1(2,2);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);

    utp::ublas::ugemm(A,B,C);

    cout << "------------\n";
    utp::ublas::utrsm(A,B);
}

