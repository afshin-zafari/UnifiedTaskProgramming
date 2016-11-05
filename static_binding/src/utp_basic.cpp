#include "utp_basic.hpp"
#include "cmdline.hpp"

#ifdef DT_INCLUDED
#include "engine.hpp"
#include "sch_ductteip.hpp"
#endif 
#ifdef SG_INCLUDED
#include "sch_superglue.hpp"
#endif

namespace utp {
    void packArgs(Args *){}
    void packAxs(Axs &){}
    void utp_initialize(int argc , char *argv[]){
      
      #ifdef DT_INCLUDED
      DT::init();
      dtEngine.start(argc,argv);
      #endif

      cmdLine.getCmdLine(argc,argv);
      
      #ifdef SG_INCLUDED
      utp::SG_Engine = new SuperGlue<utp::SGWOptions>(cmdLine.nt);
      #endif
      
    }
    void utp_finalize(){

      #ifdef DT_INCLUDED
      dtEngine.finalize();
      #endif

      
      #ifdef SG_INCLUDED
      utp::SG_Engine->barrier();
      #endif
    }
}
