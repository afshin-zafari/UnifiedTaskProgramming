#ifndef TASK_BLAS_HPP
#define TASK_BLAS_HPP
#include "IScheduler.hpp"
#include "GTask.hpp"
#include "Dispatcher.hpp"
#include "gsl/gsl_cblas.h"

void _gemm(GTask *);
void _trsm(GTask *);
#define blas_map(b) if ( t->fname == u##b##_name) _##b(t)
#endif  //TASK_BLAS_HPP


