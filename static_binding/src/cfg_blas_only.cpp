#include "cfg_blas_only.hpp"
#include "cmdline.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
void test_BLAS_ONLY(){
  int N = cmdLine.M;
  int B1= cmdLine.Mb;
    utp::GData A(N,N,"A");
    utp::GData B(N,N,"B");
    utp::GData C(N,N,"C");
    utp::GPartitioner P1(B1,B1);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);

    utp::ublas::ugemm(A,B,C);

    cout << "------------\n";
    utp::ublas::utrsm(A,B);
}

