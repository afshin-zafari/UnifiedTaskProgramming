#include <iostream>
#include "utp.hpp"
#include "cmdline.hpp"

using namespace std;
int N;
utp::GData *A,*B,*C;
utp::TimeUnit start;
/*---------------------------------------------*/
void mat_mult()
{
  N  = cmdLine.M;
  int B1 = cmdLine.Mb;
  int B2 = cmdLine.mb;
  A = new utp::GData(N,N,"A");
  B = new utp::GData(N,N,"B");
  C = new utp::GData(N,N,"C");
  utp::GPartitioner P1(B1,B1);
  utp::GPartitioner P2(B2,B2);
  P1.set_next(&P2);
  A->set_partition(&P1);
  B->set_partition(&P1);
  C->set_partition(&P1);

  A->fill_with(1.0,0.0);
  B->fill_with(2.0,0.0);
  C->fill_with(0.0,0.0);
  start = utp::UserTime();
  test_DT_SPU_BLAS(*A,*B,*C);
}
/*---------------------------------------------*/
void mat_mult_test()
{
  A->print();
  B->print();
  C->print();
  if ( config.P  ==1 ) {
    double *c = (double *)C->get_memory();
    if (  c == nullptr)
      return  ;
    double sum = 0.0;
    for(int i=0;i<N*N;i++){
      sum += c[i];
    }
    cout << " sum of C: " <<sum << endl;
  }
  delete A;
  delete B;
  delete C;
}
/*---------------------------------------------*/
/*------------------------------------------*/
void cholesky()
{
  N  = cmdLine.M;
  int B1 = cmdLine.Mb;
  int B2 = cmdLine.mb;
  cout << "Define A"<< endl << flush ;
  A = new utp::GData (N,N,"A");
  utp::GPartitioner P1(B1,B1);
  utp::GPartitioner P2(B2,B2);
  P1.set_next(&P2);
  A->set_partition(&P1);
  A->set_memory(new double [N*N],N);
  A->fill_rows_with(1,0);
  start = utp::UserTime();
  test_DT_SPU_BLAS(*A,*B,*C);
  
}
/*---------------------------------------------*/
void cholesky_test(){
  delete A;
}
/*---------------------------------------------*/
int main(int argc, char *argv[])
{
  cout << "Starts\n";

  utp::utp_initialize(argc,argv);
  cholesky();
  utp::utp_finalize();
  utp::TimeUnit end = utp::UserTime();
  cout << "Finished\n";
  cout << "execution time in ms (D SPU B with UTP): " <<  (end - start) /1000.0 << endl;
  cholesky_test();
  return 0;
}

