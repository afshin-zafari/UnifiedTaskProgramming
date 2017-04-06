#include <iostream>
#include "utp.hpp"
//#include "basic.hpp"
#include "cmdline.hpp"
using namespace std;
int me;
int main(int argc, char *argv[])
{
    cout << "Starts\n";
    utp::utp_initialize(argc,argv);
    
    int N = cmdLine.M;
    int B1= cmdLine.Mb;
    utp::GData A(N,N,"A");
    utp::GData B(N,N,"B");
    utp::GData C(N,N,"C");
    utp::GPartitioner P1(B1,B1);
    A.set_partition(&P1);
    B.set_partition(&P1);
    C.set_partition(&P1);
    A.set_memory(new double [N*N],N*N);
    B.set_memory(new double [N*N],N*N);
    C.set_memory(new double [N*N],N*N);
    A.fill_rows_with(1,10);
    B.fill_rows_with(10,10);
    C.fill_with(0,0);
    utp::TimeUnit start = utp::UserTime();
    test_BLAS_ONLY(A,B,C);

    utp::utp_finalize();
    utp::TimeUnit end = utp::UserTime();
    cout << "Finished\n";
    cout << "executione time in ms (BLAS ONLY with UTP)" << (end-start)/1000.0<< endl;
    A.print();
    B.print();
    C.print();
    
    double *c = (double *)C.get_memory();
    double sum = 0.0;
    for(int i=0;i<N*N;i++){
      sum += c[i];
    }
    cout << " sum of C: " <<sum << endl;

    return 0;
}

