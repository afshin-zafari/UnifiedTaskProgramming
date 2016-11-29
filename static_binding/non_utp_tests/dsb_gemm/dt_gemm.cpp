#include "dt_gemm.hpp"
#include "gemm.hpp"

void DTGemm::taskified(){
  int Nb = A->getXNumBlocks();
  //cout << "<<<<" << Nb << endl;
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

  if ( dt_task)
    dt_task->child_count = 0;
  Handle <Options> **A = dt_task->getDataAccess(0)->createSuperGlueHandles();
  Handle <Options> **B = dt_task->getDataAccess(1)->createSuperGlueHandles();
  Handle <Options> **C = dt_task->getDataAccess(2)->createSuperGlueHandles();
  int n = config.nb;
  //cout << "<<<" << n << endl;
  for(int i = 0; i<n; i++){
    for(int j = 0; j<n; j++){
      for(int k = 0; k<n; k++){
	GemmTask *gemm=new GemmTask(dt_task,A[i][k],B[k][j],C[i][j]);
	dtEngine.getThrdManager()->submit(gemm);
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
DTGemm::DTGemm(DuctTeip_Data *A_, DuctTeip_Data *B_, DuctTeip_Data  *C_ )
{
  name=static_cast<string>("chol");
  setParent(this);
  A  = A_;
  A->setDataHandle(createDataHandle());
  A->setParent(this);
  A->configure();

  B  = B_;
  B->setDataHandle(createDataHandle());
  B->setParent(this);
  B->configure();
  
  C  = C_;
  C->setDataHandle(createDataHandle());
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
  cout << "zzz" << endl;
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
  printf("Boundaries:  %d,%d,%d,%d\n",I,J,K,L);
  for ( int i=0; i < I; i ++ )
    {
      for ( int j=0 ; j < J ; j ++)
	{
	  if ( c(i,j)->getHost() == me )
	    {
	      cout << "C_Block: " << i << "," << j <<endl;
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
  if ( I > 10 )
    return;
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
