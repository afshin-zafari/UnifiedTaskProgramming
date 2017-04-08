#include "utp_basic.hpp"
#include "cmdline.hpp"
#include <sys/time.h>


#ifdef DT_INCLUDED
#include "engine.hpp"
#include "sch_ductteip.hpp"
#endif 
#ifdef SG_INCLUDED
#include "sch_superglue.hpp"
#endif
#ifdef SPU_INCLUDED
#include "sch_spu.hpp"
#endif

namespace utp {
    void packArgs(Args *){}
    void packAxs(Axs &){}
    void utp_initialize(int argc , char *argv[]){
      
      #ifdef DT_INCLUDED
      DT::init();
         #ifdef SG_INCLUDED
           dtEngine.start(argc,argv,true);
	   utp::SG_Engine = new SuperGlue<utp::SGWOptions>(cmdLine.nt);
	   //	   dtEngine.set_superglue(utp::SG_Engine);
         #else      
           dtEngine.start(argc,argv);
	   #ifdef SPU_INCLUDED
	   SPU::Init();
	   #endif
         #endif
      #else
	   #ifdef SPU_INCLUDED
	   SPU::Init();
	   #endif
      #endif

      cmdLine.getCmdLine(argc,argv);
      
      #ifdef SG_INCLUDED
        #ifndef DT_INCLUDED 
          utp::SG_Engine = new SuperGlue<utp::SGWOptions>(cmdLine.nt);
        #endif
      #endif
      #if DT_INCLUDED && SG_INCLUDED
      Dispatcher::Init();
      #endif
      
    }
    void utp_finalize(){

      #ifdef DT_INCLUDED
      dtEngine.finalize();
      #endif

      
      #ifdef SG_INCLUDED
      utp::SG_Engine->barrier();
      #endif
      #ifdef SPU_INCLUDED
      utp::SPU::finalize();
      #endif
    }
  

  TimeUnit UserTime() {
    timeval tv;
    TimeUnit  unit=1000000.0;
    gettimeofday(&tv, 0);
    return (tv.tv_sec*unit+tv.tv_usec);
  }

}
