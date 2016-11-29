#include "dt_gemm.hpp"

double  myUserTime() {
  timeval tv;
  double   unit=1000000.0;
  gettimeofday(&tv, 0);
  return (tv.tv_sec*unit+tv.tv_usec);
}

int main (int argc, char * argv[])
{
  DuctTeip_Start(argc,argv,false);

  DuctTeip_Data A(config.N,config.N);
  DuctTeip_Data B(config.N,config.N);
  DuctTeip_Data C(config.N,config.N);

  DTGemm *G=new DTGemm((DuctTeip_Data*)&A,
		       (DuctTeip_Data*)&B,
		       (DuctTeip_Data*)&C);
  double start = myUserTime();
  G->taskified();


  DuctTeip_Finish();

  double end = myUserTime();
  if ( me == 0){
    double t=dt_log.getStattime(DuctteipLog::ProgramExecution);
    cout << "execution time in ms (DT + SG+BLAS without UTP): " << (end - start) /  1000.0 << endl;
    fprintf(stderr,"[****] Time = %lf, N = %ld , NB= %ld , nb= %ld , p= %ld, q = %ld, gf= %lf\n",
	    t,config.N,config.Nb,config.nb,config.p,config.q,
	    double(config.N)*double(config.N)*double(config.N)/1e9/t);
  }
  G->checkCorrectness();
}
