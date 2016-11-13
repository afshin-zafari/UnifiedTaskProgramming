#include "dt_gemm.hpp"
#include "gsl/gsl_cblas.h"

void DTGemm::taskified(){
  int Nb = A->getXNumBlocks();
  DuctTeip_Data &a  = *A;
  DuctTeip_Data &b  = *B;
  DuctTeip_Data &c  = *C;
  for(int i = 0; i<Nb; i++){
    for(int j = 0; j<Nb; j++){
      for(int k = 0; k<Nb; k++){
	addTask(GEMM, a(i,k), b(k,j), c(i,j));
      }
    }
  }
}
//=======================================================
void DTGemm::runKernels(DuctTeip_Task *task ) /*@\label{line:runkernelsdef_start}@*/
{
  switch (task->getKey()){
    case  GEMM:    GEMM_kernel(task);      break;
    default:
      fprintf(stderr,"invalid task key:%ld.\n",
	      task->getKey());
      exit(1);
      break;
    }
}
/*----------------------------------------------------------------------------*/
void DTGemm::GEMM_kernel(DuctTeip_Task  *dt_task)
{
  DuctTeip_Data  *a = (DuctTeip_Data *)dt_task->getArgument(0);
  DuctTeip_Data  *b = (DuctTeip_Data *)dt_task->getArgument(1);
  DuctTeip_Data  *c = (DuctTeip_Data *)dt_task->getArgument(2);

  double *mA = a->getContentAddress();
  double *mB = b->getContentAddress();
  double *mC = c->getContentAddress();

  int Bz  = config.M / config.Mb ; 
  int ldA = Bz;
  int M   = Bz;

  int ldB = Bz;
  int N   = Bz;

  int ldC = Bz;
  int K   = Bz;
  double beta= 1.0; 
  double alpha= 1.0;
  auto TransA = CblasNoTrans;
  //Order,TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc)
  cblas_dgemm(CblasColMajor,TransA,CblasNoTrans,M,N,K,alpha,mA,ldA,mB,ldB,beta,mC,ldC);
  dt_task->setFinished(true);
  
}
/*----------------------------------------------------------------------------*/
DTGemm::DTGemm(DuctTeip_Data *A_, DuctTeip_Data *B_, DuctTeip_Data  *C_ )
{
  name=static_cast<string>("chol");
  setParent(this);
  A_->setDataHandle(createDataHandle());
      A  = A_->clone();
      A->setParent(this);
      A->configure();

  B_->setDataHandle(createDataHandle());
      B  = B_->clone();
      B->setParent(this);
      B->configure();
  C_->setDataHandle(createDataHandle());
      C  = C_->clone();
      C->setParent(this);
      C->configure();

  populateMatrice();
  addInOutData(A);
  addInOutData(B);
  addInOutData(C);
}
/*----------------------------------------------------------------------------*/
string DTGemm::getTaskName(unsigned long key)
{
  string s;
  switch(key)    {
    case  GEMM:      s.assign("gemm" ,4);      break;
  }
  return s;
}
/*----------------------------------------------------------------------------*/
void DTGemm::taskFinished(IDuctteipTask *task, TimeUnit dur)
{
  long key = task->getKey();
  double  n = config.N / config.Nb,gflops;
  switch(key) {
    case  GEMM:      gflops=(2*(n*n*n/3.0)/dur);    break;
    }
}
/*----------------------------------------------------------------------------*/
void DTGemm::checkCorrectness()
{
  if ( config.simulation) return;
  int I = cfg->getYBlocks();        // Mb
  int J = cfg->getXBlocks();        // Nb
  int K = cfg->getYDimension() / I; // #rows per block== M/ Mb
  int L = cfg->getXDimension() / J; // #cols per block== N/ Nb
  IData &c=*C;
  bool found = false;
  double exp = cfg->getYDimension() * 2;
  for ( int i=0; i < I; i ++ )
    {
      for ( int j=0 ; j < J ; j ++)
	{
	  if ( c(i,j)->getHost() == me )
	    {
	      for ( int k=0; k < K; k++)
		{
		  for ( int l=0; l <L; l++)
		    {
		      double v = c(i,j)->getElement(k,l);
		      if ( v != exp){
			printf("error in Result, exp:%lf, value=%lf\n",exp,v);
			dumpAllData();
			return;
		      }
		    }
		}
	    }
	}
    }
  printf("Result is correct\n");
}
/*----------------------------------------------------------------------------*/
void DTGemm::populateMatrice()
{
  if ( config.simulation) return;
  cfg=&config;
  int I = cfg->getYBlocks();        // Mb
  int J = cfg->getXBlocks();        // Nb
  int K = cfg->getYDimension() / I; // #rows per block== M/ Mb
  int L = cfg->getXDimension() / J; // #cols per block== N/ Nb
  IData &a=*A;
  IData &b=*B;
  IData &c=*C;
  for ( int i=0; i < I; i ++ )
    {
      for ( int j=0 ; j < J ; j ++)
	{
	  if ( a(i,j)->getHost() == me )
	    {
	      for ( int k=0; k < K; k++)
		{
		  for ( int l=0; l <L; l++)
		    {
		      a(i,j)->setElement(k,l,1.0);
		      b(i,j)->setElement(k,l,2.0);
		      c(i,j)->setElement(k,l,0.0);
		    }
		}
	    }
	}
    }

}
/*----------------------------------------------------------------------------*/
void DTGemm::dumpAllData(){
  int I = cfg->getYBlocks();        // Mb
  int J = cfg->getXBlocks();        // Nb
  int K = cfg->getYDimension() / I; // #rows per block== M/ Mb
  int L = cfg->getXDimension() / J; // #cols per block== N/ Nb
  IData &c=*C;
  bool found = false;
  double exp = cfg->getYDimension() * 2;
  for ( int i=0; i < I; i ++ )
    {
      for ( int j=0 ; j < J ; j ++)
	{
	  if ( c(i,j)->getHost() == me )
	    {
	      printf("C(%d,%d):\n",i,j);
	      for ( int k=0; k < K; k++)
		{
		  for ( int l=0; l <L; l++)
		    {
		      double v = c(i,j)->getElement(k,l);
		      printf("%2.2lf ",v);
		    }
		  printf("\n");
		}
	    }
	}
    }
}
/*----------------------------------------------------------------------------*/
DTGemm::~DTGemm()
{
}
/*----------------------------------------------------------------------------*/
