#include "cfg_blas_only.hpp"
#include "cmdline.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
void test_BLAS_ONLY(utp::GData &A,utp::GData &B, utp::GData &C){
	
    utp::ublas::ugemm(A,B,C);
}

