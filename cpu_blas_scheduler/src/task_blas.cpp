#include "task_blas.hpp"
/*---------------------------------------------------------------------------------------*/
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
	a->get_name() << "*"  <<
	b->get_name() << "->" <<
	c->get_name() << "\n---------------\n";	
      cout << A << "," << B << "," << C << endl;
      printf("%p,%p,%p\n%d,%d,%d\n%d,%d,%d\n",A,B,C,ldA,ldB,ldC,M,N,K);
      cout << "===============================\n";
      a->print();
      cout << "===============================\n";
      b->print();
      cout << "===============================\n";
    }
    cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,M,N,K,alpha,A,ldA,B,ldB,beta,C,ldC);
    if (dbg)
      cout << "-----------\n" ;

}
/*---------------------------------------------------------------------------------------*/
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

/*---------------------------------------------------------------------------------------*/
void _add ( GTask *t)
{
    GData *a = t->args->args[0];
    GData *b = t->args->args[1];

    double *A=(double *)a->get_memory();
    int mA = a->get_rows();
    int nA = a->get_cols();

    double *B=(double *)b->get_memory();
    int mB = b->get_rows();
    int nB = b->get_cols();

    LOG_INFO(LOG_MLEVEL,"A:%s %d x %d, B:%s %d x %d\n",
	     a->get_name().c_str(),mA,nA,
	     b->get_name().c_str(),mB,nB);
    if ( mA * mB  == 1){ // row vectors
      if ( nA == nB  ) { 
	cblas_daxpy(nB,t->get_alpha(),A,1,B,1);
      }
      else {
	fprintf(stderr,"%s,%s Size mismatch in arrays: A:%d x %d B:%d x %d .\n",
		__FILE__,__FUNCTION__,mA,nA,mB,nB);
      }
    }
    else if ( nA * nB  ==1 ) {//column vectors
      if ( mA == mB   ) { 
	cblas_daxpy(mB,t->get_alpha(),A,1,B,1);
      }
      else {
	fprintf(stderr,"%s,%s Size mismatch in arrays: A:%d x %d B:%d x %d.\n",
		__FILE__,__FUNCTION__,mA,nA,mB,nB);
      }
    }
    else if ( nA == nB and  mA == mB) { // matrix add
      for ( int  i=0; i< mA; i++){
	for ( int j=0; j < nA; j++){
	  B[i+j*mA] += A[i+j*mA];
	}
      }
    }
    else {
      fprintf(stderr,"%s,%s Size mismatch in arrays: A:%d x %d B:%d x %d.\n",
	      __FILE__,__FUNCTION__,mA,nA,mB,nB);
    }
}
/*---------------------------------------------------------------------------------------*/
void _copy ( GTask *t)
{
    GData *a = t->args->args[0];
    GData *b = t->args->args[1];


    double *A=(double *)a->get_memory();
    int mA = a->get_rows();
    int nA = a->get_cols();
    double *B=(double *)b->get_memory();
    int mB = b->get_rows();
    int nB = b->get_cols();
    LOG_INFO(LOG_MLEVEL,"A:%s %d x %d, B:%s %d x %d\n",a->get_name().c_str(),mA,nA,b->get_name().c_str(),mB,nB);
    if ( mB*nB <= mA*nA)
      memcpy ( B,A,mB*nB*sizeof(double));
    else
      fprintf(stderr,"%s,%s destination (%d x %d = %d)is smaller than source (%d x %d = %d).\n"
	      ,__FILE__,__FUNCTION__,mB,nB,mB*nB, mA,nA,mA*nA);
}
/*---------------------------------------------------------------------------------------*/
void _dot ( GTask *t)
{
   GData *a = t->args->args[0];
   GData *b = t->args->args[1];
   GData *c = t->args->args[2];


    double *A=(double *)a->get_memory();
    int mA = a->get_rows();
    int nA = a->get_cols();
    double *B=(double *)b->get_memory();
    int mB = b->get_rows();
    int nB = b->get_cols();
    assert(c);
    double *C=(double *)c->get_memory();
    cout << "C data : " << c->get_name() << endl;
    assert(A);
    assert(B);
    assert(C);
    cout << "-----------\n" ;
    a->print();
    cout << "-----------\n" ;
    b->print();
    cout << "-----------\n" ;
    c->print();
    cout << "-----------\n" ;
    if ( mA * mB  == 1){ // row vectors
      if ( nA == nB  ) { 
	C[0]=cblas_ddot(nA,A,1,B,1);
      }
      else {
	fprintf(stderr,"%s,%s Size mismatch in arrays: A:%d x %d B:%d x %d.\n",
		__FILE__,__FUNCTION__,mA,nA,mB,nB);
      }
    }
    else if ( nA * nB ==1 ) {//column vectors
      if ( mA == mB   ) { 
	 C[0]=cblas_ddot(mA,A,1,B,1);
      }
      else {
	fprintf(stderr,"%s,%s Size mismatch in arrays: A:%d x %d B:%d x %d.\n",
		__FILE__,__FUNCTION__,mA,nA,mB,nB);
      }
    }
    else
      fprintf(stderr,"%s,%s input arguments are nopt vector  A: %d x %d , B: %d x %d .\n"
	      ,__FILE__,__FUNCTION__,mA,nA, mB,nB);
    c->print();
    cout << "-----------" << C[0] << '\n' ;
}
