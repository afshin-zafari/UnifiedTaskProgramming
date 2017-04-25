#include "cfg_spu_blas.hpp"
#include "cmdline.hpp"

#include "upotrf.hpp"

void test_SPU_BLAS(utp::GData &A, utp::GData &B, utp::GData &C){
  //if(0)utp::ublas::ugemm(A,B,C);
  utp::ublas::upotrf(A);
}
