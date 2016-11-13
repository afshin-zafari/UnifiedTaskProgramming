#include <iostream>

#include "sg/superglue.hpp"
#include "sg/option/instr_trace.hpp"
#include "gsl/gsl_cblas.h"
#include <sys/time.h>

timeval start,finish;
void tic()
{
    gettimeofday(&start,NULL);
}
double toc()
{
    gettimeofday(&finish,NULL);
    return ((finish.tv_sec-start.tv_sec)*1000000+finish.tv_usec-start.tv_usec)/1000000.0;
}


using namespace std;
double **Adata,**Bdata,**Cdata;
size_t N, NB, DIM;

// Expand handle to include two indexes
template<typename Options>
struct MyHandle : public HandleBase<Options> {
    size_t i, j;
    void set(size_t i_, size_t j_) { i = i_; j = j_; }
    size_t geti() { return i; }
    size_t getj() { return j; }
};

// ==========================================================================
// Task Library Options
//
// Set task library options:
//   * Use user expanded handles (that includes two indexes)
//   * Enable logging (generate execution trace)
// ==========================================================================
struct Options : public DefaultOptions<Options> {
    typedef MyHandle<Options> HandleType;
  //    typedef Trace<Options> Instrumentation;
    typedef Enable TaskName;
};


/*
static void convert_to_blocks(double *Alin, double **A) {
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            A[(j/NB)*DIM + i/NB][(i%NB)*NB+j%NB] = Alin[i*N+j];
}
*/

void fill_random(double *Alin, int NN) {
    for (int i = 0; i < NN; i++)
        Alin[i] = ((double)rand())/((double)RAND_MAX);
}

struct gemm : public Task<Options, -1> {
    gemm(Handle<Options> &h1, Handle<Options> &h2, Handle<Options> &h3) {
        register_access(ReadWriteAdd::read, h1);
        register_access(ReadWriteAdd::read, h2);
        register_access(ReadWriteAdd::write, h3);
    }
    void run() {

        double *a(Adata[get_access(0).get_handle()->geti()*DIM + get_access(0).get_handle()->getj()]);
        double *b(Bdata[get_access(1).get_handle()->geti()*DIM + get_access(1).get_handle()->getj()]);
        double *c(Cdata[get_access(2).get_handle()->geti()*DIM + get_access(2).get_handle()->getj()]);

        int nb=NB;
        double DONE=1.0;


        cblas_dgemm(CblasColMajor,CblasNoTrans, CblasNoTrans, nb, nb, nb, DONE, a, nb, b, nb, DONE, c, nb);
    }
    std::string get_name() { return "gemm"; }
};
static double matmul(const int num_threads, const size_t numBlocks) {

    SuperGlue<Options> sg(num_threads);

    Handle<Options> **A = new Handle<Options>*[numBlocks];
    for (size_t i = 0; i < numBlocks; ++i)
        A[i] = new Handle<Options>[numBlocks];

    for (size_t i = 0; i < numBlocks; ++i)
        for (size_t j = 0; j < numBlocks; ++j)
            A[i][j].set(i, j);

    Handle<Options> **B = new Handle<Options>*[numBlocks];
    for (size_t i = 0; i < numBlocks; ++i)
        B[i] = new Handle<Options>[numBlocks];

    for (size_t i = 0; i < numBlocks; ++i)
        for (size_t j = 0; j < numBlocks; ++j)
            B[i][j].set(i, j);

    Handle<Options> **C = new Handle<Options>*[numBlocks];
    for (size_t i = 0; i < numBlocks; ++i)
        C[i] = new Handle<Options>[numBlocks];

    for (size_t i = 0; i < numBlocks; ++i)
        for (size_t j = 0; j < numBlocks; ++j)
            C[i][j].set(i, j);

    tic();
    Time::TimeUnit t1= Time::getTime();
    for (size_t i = 0; i < numBlocks; i++) {
        for (size_t j = 0; j < numBlocks; j++) {
            for (size_t k = 0; k < numBlocks; k++) {
                // C[i,j] = C[i,j] + A[i,k] * (B[j,k])
                sg.submit(new gemm(A[i][k], B[k][j], C[i][j]));
            }
        }
    }
    sg.barrier();
    Time::TimeUnit t2= Time::getTime();
    cout << "getTime(ms): " << (t2-t1)/3.0e6 << endl;
    return toc();
}

int main(int argc, char **argv)
{
    if (argc >= 3) {
        NB = (long) atoi(argv[1]);
        DIM = (long) atoi(argv[2]);
    }
    else {
        printf("usage: %s NB DIM [num_cores]\n\n", argv[0]);
        printf("  example: %s 256 16\n", argv[0]);
        exit(0);
    }
    N = NB*DIM;

    int num_workers = -1;
    if (argc >= 4)
        num_workers = atoi(argv[3]);

    // fill the matrix with random values
    /*
    double *Alin = (double *) malloc(N*N * sizeof(double));
    double *Blin = (double *) malloc(N*N * sizeof(double));
    double *Clin = (double *) malloc(N*N * sizeof(double));
    */
    


    // blocked matrix
    Adata = (double **) malloc(DIM*DIM*sizeof(double *));   // A: DIM x DIM
    for (size_t i = 0; i < DIM*DIM; i++){
        Adata[i] = (double *) malloc(NB*NB*sizeof(double)); // A[i][j]: NB x NB
	fill_random(Adata[i], NB*NB);

    }

    //convert_to_blocks(Alin, Adata);

    Bdata = (double **) malloc(DIM*DIM*sizeof(double *));   
    for (size_t i = 0; i < DIM*DIM; i++){
        Bdata[i] = (double *) malloc(NB*NB*sizeof(double)); 
	fill_random(Bdata[i], NB*NB);
    }

    //convert_to_blocks(Blin, Bdata);

    Cdata = (double **) malloc(DIM*DIM*sizeof(double *));   
    for (size_t i = 0; i < DIM*DIM; i++){
        Cdata[i] = (double *) malloc(NB*NB*sizeof(double)); 
	fill_random(Cdata[i], NB*NB);
    }

    //convert_to_blocks(Clin, Cdata);

    //fill_random(Cdata, N*N);
    double t = matmul(num_workers,DIM);

    cout << "SG Version. N: "  << DIM * NB
        << ", B: " << DIM
        << ", time: " << t
    << endl;
    //    Options::Instrumentation::dump("execution.log");
    return 0;
}
