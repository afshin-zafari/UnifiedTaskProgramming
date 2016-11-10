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

  cout << argc << endl;
  static struct option opts[]=
    {
      {"dyn-load-bal" ,no_argument      , 0,'D'}, // 0
      {"dlb"          ,no_argument      , 0,'D'},
      {"BLAS"         ,no_argument      , 0,'L'}, // 2
      {"blas"         ,no_argument      , 0,'L'},
      {"num-proc"     ,required_argument, 0,'P'}, // 4
      {"proc-grid-row",required_argument, 0,'p'},
      {"proc-grid-col",required_argument, 0,'q'}, // 6
      {"data-rows"    ,required_argument, 0,'N'},
      {"data-cols"    ,required_argument, 0,'M'}, // 8
      {"num-threads"  ,required_argument, 0,'t'},
      {"inst-per-node",required_argument, 0,'I'}, // 10
      {"ipn"          ,required_argument, 0,'I'},
      {"timeout"      ,required_argument, 0,'T'}, // 12
      {"poll-sleep"   ,required_argument, 0,'S'}, //
      {"simulation"   ,no_argument      , 0,'U'}, // 14
      {"silent-dur"   ,required_argument, 0,'u'}, // 15
      {"dlb-threshold",required_argument, 0,'r'}, // 16
      {"dlb-smart"    ,no_argument      , 0,'m'}, // 17
      {"scheduler1"   ,required_argument, 0,'x' }, // 18
      {"scheduler2"   ,required_argument, 0,'y' }, // 19
      {"scheduler3"   ,required_argument, 0,'z' }, // 20
      {"sched-lib1"   ,required_argument, 0,'X' }, // 21
      {"sched-lib2"   ,required_argument, 0,'Y' }, // 22
      {"sched-lib3"   ,required_argument, 0,'Z' }, // 23
      {"mq-send"      ,required_argument, 0,24  }, // 24
      {"mq-recv"      ,required_argument, 0,25  }, // 25
      {"mq-ip"        ,required_argument, 0,26  }, // 26
      {"mq-name"      ,required_argument, 0,27  }, // 27
      {"mq-pass"      ,required_argument, 0,28  }, // 28
      {0,0,0,0}
    };
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

