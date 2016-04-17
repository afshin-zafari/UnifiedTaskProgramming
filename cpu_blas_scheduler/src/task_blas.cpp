#include "task_blas.hpp"
void _gemm(GTask *t)
{
    GData *a = t->args->args[0];
    GData *b = t->args->args[1];
    GData *c = t->args->args[2];


    double *A=(double *)a->get_memory();
    int ldA = a->get_rows();
    int M = a->get_rows();

    double *B=(double *)b->get_memory();
    int ldB = b->get_rows();
    int N = b->get_cols();

    double *C=(double *)c->get_memory();
    int ldC = c->get_rows();
    int K = a->get_cols();
    double beta= t->get_beta();
    double alpha= t->get_alpha();
    auto TransA = t->get_trans_a()?CblasTrans:CblasNoTrans;
    //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
    bool dbg = false;

    if (dbg) {
      cout << "-----------\n" <<
	a->getName() << "*"  <<
	b->getName() << "->" <<
	c->getName() << "\n---------------\n";	
      cout << A << "," << B << "," << C << endl;
      printf("%p,%p,%p\n%d,%d,%d\n%d,%d,%d\n",A,B,C,ldA,ldB,ldC,M,N,K);
      cout << "===============================\n";
      a->print();
      cout << "===============================\n";
      b->print();
      cout << "===============================\n";
    }
    //    cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,M,N,K,alpha,A,ldA,B,ldB,beta,C,ldC);
    if (dbg)
      cout << "-----------\n" ;

}
void _trsm(GTask *t)
{
    GData *a = t->args->args[0];
    GData *b = t->args->args[1];
    printf("trsm------------, B(%d) \n",b->get_child_index());


    double *A=(double *)a->get_memory();
    int ldA = a->get_rows();

    double *B=(double *)b->get_memory();
    int ldB = b->get_rows();
    int M = b->get_rows();
    int N = b->get_cols();

    //cblas_dtrsm (Order, Side, Uplo, TransA, Diag, M, N, alpha, A, lda, B, ldb)
    printf("trsm------------, B(%d) \n",b->get_child_index());
    b->dump();printf("--\n");
    cblas_dtrsm(CblasColMajor,CblasRight,CblasLower,CblasTrans,CblasNonUnit,M,N,1.0,A,ldA,B,ldB);
    a->dump();printf("--\n");
    b->dump();printf("--\n");
}

