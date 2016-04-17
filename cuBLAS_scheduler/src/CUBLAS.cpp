#include "CUBLAS.hpp"
#include <cstdio>
#include  "unified_blas.hpp"




CUBLAS::CUBLAS(int i):IScheduler(i)
{
    cublasStatus_t status;
    status = cublasCreate(&handle);
    printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);
    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! shutdown error (A)\n");
        err= EXIT_FAILURE;
    }
}

int CUBLAS::init()
{
    return 0;
}
CUBLAS::~CUBLAS()
{
    cublasStatus_t status;

    printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);
    /* Shutdown */
    status = cublasDestroy(handle);

    printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);

    // cudaDeviceReset causes the driver to clean up all state. While
    // not mandatory in normal operation, it is good practice.  It is also
    // needed to ensure correct operation when the application is being
    // profiled. Calling cudaDeviceReset causes all profile data to be
    // flushed before the application exits
    cudaDeviceReset();
    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! shutdown error (A)\n");
        err= EXIT_FAILURE;
    }

    //dtor
}

CUBLAS::CUBLAS(const CUBLAS& other):IScheduler(-1)
{
    //copy ctor
}

CUBLAS& CUBLAS::operator=(const CUBLAS& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

void CUBLAS::submitTask(GTask *t)
{
    if ( t== nullptr)
    {
        return;
    }

}
int CUBLAS::gemm(GTask *t)
{
  cublasStatus_t status;
    printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);
  GData *a = t->args->args[0];
  GData *b = t->args->args[1];
  GData *c = t->args->args[2];
  double *h_A = (double*)a->get_memory();
  double *h_B = (double*)b->get_memory();
  double *h_C = (double*)c->get_memory();
  double *d_A,*d_B,*d_C;
  int N = a->get_rows();
  double alpha=1.0, beta=1.0;
  int n2 =  a->get_element_count();

    printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);
    if (cudaMalloc((void **)&d_A, n2 * sizeof(d_A[0])) != cudaSuccess)
    {
        fprintf(stderr, "!!!! device memory allocation error (allocate A)\n");
        return EXIT_FAILURE;
    }

    if (cudaMalloc((void **)&d_B, n2 * sizeof(d_B[0])) != cudaSuccess)
    {
        fprintf(stderr, "!!!! device memory allocation error (allocate B)\n");
        return EXIT_FAILURE;
    }

    if (cudaMalloc((void **)&d_C, n2 * sizeof(d_C[0])) != cudaSuccess)
    {
        fprintf(stderr, "!!!! device memory allocation error (allocate C)\n");
        return EXIT_FAILURE;
    }
    status = cublasSetVector(n2, sizeof(h_A[0]), h_A, 1, d_A, 1);

    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! device access error (write A)\n");
        return EXIT_FAILURE;
    }

    status = cublasSetVector(n2, sizeof(h_B[0]), h_B, 1, d_B, 1);

    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! device access error (write B)\n");
        return EXIT_FAILURE;
    }

    status = cublasSetVector(n2, sizeof(h_C[0]), h_C, 1, d_C, 1);

    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! device access error (write C)\n");
        return EXIT_FAILURE;
    }

    /* Performs operation using cublas */
    status = cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, N, N, N, &alpha, d_A, N, d_B, N, &beta, d_C, N);

    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! kernel execution error.\n");
        return EXIT_FAILURE;
    }

    /* Read the result back */
    status = cublasGetVector(n2, sizeof(h_C[0]), d_C, 1, h_C, 1);

    if (status != CUBLAS_STATUS_SUCCESS)
    {
        fprintf(stderr, "!!!! device access error (read C)\n");
        return EXIT_FAILURE;
    }

    if (cudaFree(d_A) != cudaSuccess)
    {
        fprintf(stderr, "!!!! memory free error (A)\n");
        return EXIT_FAILURE;
    }

    if (cudaFree(d_B) != cudaSuccess)
    {
        fprintf(stderr, "!!!! memory free error (B)\n");
        return EXIT_FAILURE;
    }

    if (cudaFree(d_C) != cudaSuccess)
    {
        fprintf(stderr, "!!!! memory free error (C)\n");
        return EXIT_FAILURE;
    }



    //printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);
}
void CUBLAS::runTask(GTask *t)
{
  int err = gemm(t);
  Dispatcher *d = get_dispatcher();
  assert(d);
  d->finished_task(t);
}

extern "C" {
    IScheduler *f_create(int id)
    {
        return new CUBLAS(id);
    }
    void f_destroy(IScheduler *s)
    {
        delete s;
    }
}
void CUBLAS::finishedTask(GTask *t)
{
}
void CUBLAS::finalize()
{
}
void CUBLAS::allocate_memory(GData *d)
{
}
void CUBLAS::data_created(GData *)
{
}
void CUBLAS::data_partitioned(GData *)
{
}
