#ifndef CUBLAS_HPP
#define CUBLAS_HPP
#include "IScheduler.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include "Dispatcher.hpp"

/* Includes, cuda */
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <helper_cuda.h>


class CUBLAS : public IScheduler
{
public:
    CUBLAS(int);
    virtual ~CUBLAS();
    CUBLAS(const CUBLAS& other);
    CUBLAS& operator=(const CUBLAS& other);
    void submitTask(GTask *);
    void runTask(GTask *);
    void finishedTaskGroup(GTask *) {}
    void finishedTask(GTask *);
    void finalize();
    int init();
    void allocate_memory(GData *);
    void data_created(GData *);
    void data_partitioned(GData *);
    int gemm(GTask *);
protected:
private:
    cublasHandle_t handle;
  int err;

};

#endif // CUBLAS_HPP
