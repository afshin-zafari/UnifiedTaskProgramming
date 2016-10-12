#include <iostream>
#include "unified_blas.hpp"
#include "Dispatcher.hpp"
#include "ductteip.hpp"
#include "config.hpp"
#include <map>
#include <cstdio>
#include <sys/time.h>
static inline TimeUnit  UserTime(int y) {
  timeval tv;
  unsigned long  unit = 1000000;
  gettimeofday(&tv, 0);
  return (tv.tv_sec*unit+tv.tv_usec/unit);
}


map<string,GOperation *> op;
using namespace std;
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
GData *test_gemm()
{
    int M = config.getYDimension();
    int YB1 = config.getYBlocks();
    int YB2 = config.getYLocalBlocks();
    GData *_A = new GData(M,M,"A");
    GData *_B = new GData(M,M,"B");
    GData *_C = new GData(M,M,"C");
    GData &A=*_A,&B=*_B,&C=*_C;

    GPartitioner *P1= new GPartitioner(YB1,YB1);
    GPartitioner *P2= new GPartitioner(YB2,YB2);
    P1->set_next(P2);
    cout << "+++++++++++++"<<
      A.get_name() << "," <<
      B.get_name() << "," <<
      C.get_name() << "." << endl;

    printf("Partitioning starts now %dx%d\n",YB1,YB1);

    A.set_partition(P1);
    B.set_partition(P1);
    C.set_partition(P1);
    A.fill_with(1.0);
    B.fill_with(2.0);
    cout << "+++++++++++++"<<endl;
    B.print();
    cout << "+++++++++++++"<<endl;
    C.fill_with(10.0);
    cout << "+++++++++++++"<<endl;
    tic();
    ugemm(A,B,C);
    return _C;
}
int main(int argc, char **argv)
{
    utp_initialize(argc,argv);

    GData *C = test_gemm();

    utp_finalize();

    cout << "Unified Version. N: " << config.getYDimension()
        << ", B: " << config.getYBlocks()
        << ", time: " << toc()
        << endl;

    C->print();
    cout << "Program finished.\n";
    return 0;
}

