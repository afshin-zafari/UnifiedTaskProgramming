#include "dt_gemm.hpp"

int main (int argc, char * argv[])
{
  DuctTeip_Start(argc,argv);

  DuctTeip_Data A(config.N,config.N);
  DuctTeip_Data B(config.N,config.N);
  DuctTeip_Data C(config.N,config.N);

  DTGemm *G=new DTGemm((DuctTeip_Data *)&A,
		       (DuctTeip_Data *)&B,
		       (DuctTeip_Data *)&C);
  TimeUnit start = UserTime();
  G->taskified();


  DuctTeip_Finish();

  TimeUnit end = UserTime();
  if ( me == 0){
    double t=dt_log.getStattime(DuctteipLog::ProgramExecution);
    cout << "execution time in ms (DT + BLAS without UUTP): " << (end - start) /  3000.0 << endl;
    fprintf(stderr,"[****] Time = %lf, N = %ld , NB= %ld , nb= %ld , p= %ld, q = %ld, gf= %lf\n",
	    t,config.N,config.Nb,config.nb,config.p,config.q,
	    double(config.N)*double(config.N)*double(config.N)/3e9/t);
  }
  G->checkCorrectness();
}
