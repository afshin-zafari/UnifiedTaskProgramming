#include "cfg_dt_blas.hpp"
#include "config.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"

void test_DT_BLAS(utp::GData &A,utp::GData &B,utp::GData &C){
    utp::ublas::ugemm(A,B,C);


    std::cout << "------------\n";
    //utp::ublas::utrsm(A,B);
    
}
