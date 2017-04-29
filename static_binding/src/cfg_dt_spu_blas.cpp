#include "cfg_dt_sg_blas.hpp"
#include "config.hpp"

#include "upotrf.hpp"
void test_DT_SPU_BLAS(utp::GData &A,utp::GData &B,utp::GData &C){
  
    utp::ublas::upotrf(A);

}
