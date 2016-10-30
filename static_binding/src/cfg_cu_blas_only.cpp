#include "cfg_cu_blas_only.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
void test_cuBLAS_ONLY(){
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

    cout << "------------\n";
    utp::ublas::utrsm(A,B);
}


