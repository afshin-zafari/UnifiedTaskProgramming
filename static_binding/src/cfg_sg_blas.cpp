#include "cfg_sg_blas.hpp"
#include "cmdline.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"

void test_SG_BLAS(utp::GData &A, utp::GData &B, utp::GData &C){

    utp::ublas::ugemm(A,B,C);


    cout << "-----------------\n";

    //utp::ublas::utrsm(A,B);
}
