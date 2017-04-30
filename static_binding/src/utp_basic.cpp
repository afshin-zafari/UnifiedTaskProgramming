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
#include <cusolverDn.h>
#endif

namespace utp {
  #ifdef SPU_INCLUDED
    extern cusolverDnHandle_t *cusolver_handle;
  #endif
    void packArgs(Args *){}
    void packAxs(Axs &){}
    void utp_initialize(int argc , char *argv[]){

      cmdLine.getCmdLine(argc,argv);
      
      #ifdef DT_INCLUDED
      DT::init();
         #ifdef SG_INCLUDED
           dtEngine.start(argc,argv,true);
	   utp::SG_Engine = new SuperGlue<utp::SGWOptions>(cmdLine.nt);
	   //	   dtEngine.set_superglue(utp::SG_Engine);
         #else      
           dtEngine.start(argc,argv,true);
	   #ifdef SPU_INCLUDED
	   SPU::Init();
	   #if WITH_CUSOLVER==1
	   cout << "cu solver create\n";
	   cusolver_handle = new cusolverDnHandle_t;
	   cusolverDnCreate(cusolver_handle);
	   cout << "cu solver create finished\n";
	   #endif
           #endif
         #endif
      #else
	   #ifdef SPU_INCLUDED
	   SPU::Init();
	   #if WITH_CUSOLVER==1
	   cout << "cu solver create\n";
	   cusolver_handle = new cusolverDnHandle_t;
	   cusolverDnCreate(cusolver_handle);
	   cout << "cu solver create finished\n";
	   #endif
	   #endif
      #endif

      
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
      cout << "destroy cu solver." << cusolver_handle << endl << flush ;
      if (cusolver_handle)
	cusolverDnDestroy(*cusolver_handle);
      cout << "after destroy cu solver.\n" ;
      #endif
    }
  

  TimeUnit UserTime() {
    timeval tv;
    TimeUnit  unit=1000000.0;
    gettimeofday(&tv, 0);
    return (tv.tv_sec*unit+tv.tv_usec);
  }

}
