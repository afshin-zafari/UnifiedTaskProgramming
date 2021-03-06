#include <iostream>
#include "utp.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    cout << "Starts\n";

    utp::utp_initialize(argc,argv);
    int N  = config.M;
    int B1 = config.Mb;
    int B2 = config.mb;
    utp::GData A(N,N,"A");
    utp::GData B(N,N,"B");
    utp::GData C(N,N,"C");
    utp::GPartitioner P1(B1,B1);
    utp::GPartitioner P2(B2,B2);
    P1.set_next(&P2);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);

    A.fill_with(1.0,0.0);
    B.fill_with(2.0,0.0);
    C.fill_with(0.0,0.0);
    utp::TimeUnit start = utp::UserTime();
    test_DT_SG_BLAS(A,B,C);

    utp::utp_finalize();
    utp::TimeUnit end = utp::UserTime();
    cout << "Finished\n";
    cout << "execution time in ms (DSB with UTP): " <<  (end - start) /1000.0 << endl;
    A.print();
    B.print();
    C.print();
    if ( config.P  ==1 ) {
      double *c = (double *)C.get_memory();
      if (  c == nullptr)
	return 0 ;
      double sum = 0.0;
      for(int i=0;i<N*N;i++){
	sum += c[i];
      }
      cout << " sum of C: " <<sum << endl;
    }
    return 0;
}

