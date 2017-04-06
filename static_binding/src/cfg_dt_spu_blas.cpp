#include "cfg_dt_sg_blas.hpp"
#include "config.hpp"

#include "ugemm.hpp"
#include "utrsm.hpp"

void test_DT_SPU_BLAS(utp::GData &A,utp::GData &B,utp::GData &C){
  
    utp::ublas::ugemm(A,B,C);

}
