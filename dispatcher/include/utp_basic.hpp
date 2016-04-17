#ifndef UTP_BASIC_HPP
#define UTP_BASIC_HPP
#include <vector>
#include <cassert>


#define SLIB 1

extern const int In;
extern const int Out;
extern const int InOut;

using namespace std;
typedef unsigned long ulong;
typedef unsigned int uint;
typedef ulong FUT; // Folow Up Ticket
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
struct Args{
  vector<GData*> args;
  //todo: access to be member of Args. -> thread safety
  Args(){args.reserve(5);}
  void addArg(GData *a){
    args.push_back(a);
  }
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
#endif //UTP_BASIC_HPP
