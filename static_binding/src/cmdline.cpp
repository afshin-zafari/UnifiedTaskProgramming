#include "cmdline.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

#include <iostream>
#include <cstring>
using namespace std;
CommandLine  cmdLine;
/*-----------------------------------------------------------------*/
CommandLine::CommandLine (){
  P=p=q=N=M=Mb=nb=Nb=nt=ipn=to=ps=-1;
  simulation=dlb=false;
  dlb_thr=10;
  sil_dur=1000;
  column_major=using_blas=true;
}
/*-----------------------------------------------------------------*/
void CommandLine::setParams( int n,int m ,
			int ynb , int xnb ,
			int P_, int p_, int q_,
			int mb_, int nb_,
			int nt_,
			bool dlb_,int ipn_,bool blas ){
  N=n;M=m;
  Nb=xnb;Mb=ynb;
  P=P_;p=p_;q=q_;
  nb=nb_;mb=mb_;
  nt=nt_;
  dlb=dlb_;ipn=ipn_;
  using_blas = blas;
  column_major = using_blas;
  row_major = !column_major;
}
/*-----------------------------------------------------------------*/
CommandLine::CommandLine ( int n,int m , int _nb , int _mb , int P_, int p_,
		 int q_,int nb_, int mb_, int nt_,bool dlb_,int ipn_):
  N(n),M(m),Nb(_nb),Mb(_mb),
  P(P_),p(p_),q(q_),
  mb(mb_),nb(nb_),
  nt(nt_),ipn(ipn_),dlb(dlb_){}

/*-----------------------------------------------------------------*/
void CommandLine::getCmdLine(int argc, char **argv){

  int index=1;
  while(index < argc){
    
    if ( strcmp(argv[index], "-M") == 0  ) {
      M  = atoi(argv[++index]);
      Mb = atoi(argv[++index]);
      mb = atoi(argv[++index]);
      continue;
    }
    if ( strcmp(argv[index], "-N") == 0  ) {
      N  = atoi(argv[++index]);
      Nb = atoi(argv[++index]);
      nb = atoi(argv[++index]);
      continue;
    }
    if ( strcmp(argv[index], "-P") == 0 ) {  P = atoi(argv[++index]); continue; }
    if ( strcmp(argv[index], "-p") == 0 ) {  p = atoi(argv[++index]); continue; }
    if ( strcmp(argv[index], "-q") == 0 ) {  q = atoi(argv[++index]); continue; }
    if ( strcmp(argv[index], "-T") == 0 ) { to = atoi(argv[++index]); continue; }
    if ( strcmp(argv[index], "-t") == 0 ) { nt = atoi(argv[++index]); continue; }
  index++;
  }
}

