#ifndef UTP_BASIC_HPP
#define UTP_BASIC_HPP
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#if DEBUG != 0
#define PRINTF(x,y...) printf(x,##y);
#else
#define PRINTF(x,y...)
#endif



namespace utp{
  typedef double TimeUnit;
  TimeUnit UserTime() ;
  const int In    = 0;
  const int Out   = 1;
  const int InOut = 2;

  using namespace std;
  typedef unsigned long ulong;
  typedef unsigned int uint;
  typedef unsigned char byte;

  class GenTask;
  class GData;
  class GenLevel;

  //typedef Handle<Options> sg_data_t;
  //typedef IData dt_data_t;
  struct GenHandle{
    ulong handle;
    GenHandle(ulong h):handle(h){}
  };
  /*====================================================================*/
  struct Axs{
    vector<int> axs;
    Axs(){axs.reserve(5);}
    void addAxs(int a){
      axs.push_back(a);
    }
  };
  /*====================================================================*/
  struct Args{
    vector<GData*> args;
    Axs  axs;

    Args(){}
    void addArg(GData *a){
      args.push_back(a);
    }
  };
  /*====================================================================*/

  void packArgs(Args *a);
  void packAxs (Axs  &a);
  template <typename T, typename... P>
  void packArgs(Args *a,T& arg, P&... args){
    a->addArg(&arg);
    packArgs(a,args...);
  }
  template <typename T, typename... P>
  void packAxs(Axs &a,T axs, P... args){
    a.addAxs(axs);
    packAxs(a,args...);
  }

  void utp_initialize(int argc, char *argv[]);
  void utp_finalize();
  template<class T>
  void copy(byte * b,int &o,T a){
    memcpy(b+o,(char *)&a,sizeof(a));
    o +=  sizeof(a);
  }
  template<class T>
  void paste(byte * b,int &o,T *a){
    memcpy((char *)a,b+o,sizeof(T));
    o +=  sizeof(T);
  }


}//namespace utp
#endif //UTP_BASIC_HPP
