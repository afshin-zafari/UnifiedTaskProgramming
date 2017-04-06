#ifndef CONFIG_SCHEDULERS_HPP
#define CONFIG_SCHEDULERS_HPP


#ifdef BLAS_ONLY
    #include "cfg_blas_only.hpp"
#endif // BLAS_ONLY

#ifdef CUBLAS_ONLY
    #include "cfg_cu_blas_only.hpp"
#endif // CUBLAS_ONLY

#ifdef SG_BLAS
    #include "cfg_sg_blas.hpp"
#endif // SG_BLAS

#ifdef SPU_BLAS
    #include "cfg_spu_blas.hpp"
#endif // SPU_BLAS


#ifdef DT_BLAS
    #include "cfg_dt_blas.hpp"
#endif // DT_BLAS

#ifdef DT_SG_BLAS
#include "cfg_dt_sg_blas.hpp"
#endif // DT_SG_BLAS

#ifdef DT_SPU_BLAS
#include "cfg_dt_spu_blas.hpp"
#endif // DT_SPU_BLAS

#ifdef DT_FORK_SG_SPU
#include "cfg_dt_fork_sg_spu.hpp"
#endif // DT_FORK_SG_SPU

#endif // CONFIG_SCHEDULERS_HPP
