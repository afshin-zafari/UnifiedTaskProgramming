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
    A.set_memory(new double [N*N]);
    B.set_memory(new double [N*N]);
    C.set_memory(new double [N*N]);
    A.fill_rows_with(1,10);
    B.fill_rows_with(10,10);
    C.fill_with(0);
	
    utp::ublas::ugemm(A,B,C);
    A.print();
    B.print();
    C.print();
    cout << "------------\n";
    utp::ublas::utrsm(A,B);
}

