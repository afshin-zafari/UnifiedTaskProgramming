#ifndef SCH_SPU_HPP
#define SCH_SPU_HPP
#include "operation.hpp"
#include "dispatcher.hpp"
#include "sch_ductteip.hpp"
#include "starpu.h"

#define FPRINTF(ofile, fmt, ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ## __VA_ARGS__); }} while(0)
#if UTP_DEBUG==0
bool debug=false;
#else
bool debug=true;
#endif
namespace utp{
  template <typename T> class OperationBase;
  template <typename T,typename P> class Task;
  typedef double ElementType;
  class SPU;
  extern SPU _spu;
  /*===============================================================================================*/
  class SPUData{
  public:
    starpu_data_handle_t handle;
    GData *gdata;
  /*-----------------------------------------------------------------------------------------*/
    SPUData(GData *x){
      gdata = x;
      int M = gdata->get_rows();
      int N = gdata->get_cols();
      ElementType *mem = (ElementType *)gdata->get_memory();
      if ( mem ){
	starpu_matrix_data_register(&handle, 0, (uintptr_t)mem, N, N, M, sizeof(ElementType));
	if(debug)cout << "Data " << x->get_name() << " registered at SPU with mem  " << mem <<" and handle " << handle << endl<< flush;
      }
    }
  /*-----------------------------------------------------------------------------------------*/
    void register_mem(double *mem,int ld){
      if ( mem ){
	int M = gdata->get_rows();
	int N = gdata->get_cols();;
	starpu_matrix_data_register(&handle, 0, (uintptr_t)mem, N, N, M, sizeof(ElementType));
	if(debug)cout << "Data " << gdata->get_name() << " registered at SPU with mem: " << mem << " and handle: " << handle << endl<< flush;
      }
    }
  /*-----------------------------------------------------------------------------------------*/
    void destroy(){
      starpu_data_unregister(handle ) ;
    }
  /*-----------------------------------------------------------------------------------------*/
    ~SPUData(){ destroy();  }
  };
  /*===============================================================================================*/
  template<typename T, typename P>
  class SPUTask{
  public:
    struct starpu_codelet *clp;


    typedef Task<T,P> GTask;
    GTask *gtask;
  /*-----------------------------------------------------------------------------------------*/
    SPUTask(GTask *t):gtask(t){
      clp = new starpu_codelet;
      starpu_codelet_init(clp);
      starpu_codelet &cl = *clp;
      cl. cpu_funcs[0] = SPUTask::run;
      cl.cuda_funcs[0] = SPUTask::run_cuda;
      cl.cuda_flags[0] = STARPU_CUDA_ASYNC;
      cl.nbuffers = gtask->args->args.size();
      if(debug)cout << "SPUTask created with arg count: " << cl.nbuffers << endl << flush;
      for(int i=0;i<cl.nbuffers;i++){
	if      (gtask->axs->axs[i] == In    ) cl.modes[i] =STARPU_R;
	else if (gtask->axs->axs[i] == Out   ) cl.modes[i] =STARPU_W;
	else if (gtask->axs->axs[i] == InOut ) cl.modes[i] =STARPU_RW;
	if(debug)cout << "access of arg " << i << " = " << cl.modes[i] << endl << flush;
      }
      cl.name = "general_spu_cl";
    }
  /*-----------------------------------------------------------------------------------------*/
    starpu_task *get_spu_task(){
      if(debug)cout << "get_spu_task called\n" << flush;
      starpu_task *spu_task = starpu_task_create();
      if(debug)cout << "after starpu_task_create\n" << flush;
      assert(spu_task);
      spu_task->cl = clp;
      spu_task->cl_arg = (void  *) gtask;
      spu_task->cl_arg_size = sizeof(gtask);
      if(debug)cout << "No of args: " << gtask->args->args.size() << endl;
      for(int i=0;i<clp->nbuffers;i++){
	if(debug)cout << "argument " << i << endl << flush;
	assert(gtask);
	assert(gtask->args);
	assert(gtask->args->args[i]);	
	SPUData *a = static_cast<SPUData *>(gtask->args->args[i]->get_guest());
	assert(a);
	spu_task->handles[i] = a->handle;
	if(debug)
	  cout << "Arg data " << gtask->args->args[i]->get_name() <<
	    " handle of spu data " << a->handle << endl << flush;
      }
      if(debug)cout << "get_spu_task returned\n" << flush;
      return spu_task;
    }
  /*-----------------------------------------------------------------------------------------*/
    static void run(void *descr[], void *_args){
      GTask *t= (GTask *)_args;
      int n=t->args->args.size()-1;
      if(debug)cout << "SPUTask is ready to run for " << t->args->args[n]->get_name()<< endl << flush;
#if SHORTCUT == 0
      Dispatcher::ready(_spu,t);
#else
      if ( t )
	t->o->run(t);
      else
	printf("null task *\n");
#endif
      
    }
  /*-----------------------------------------------------------------------------------------*/
    static void run_cuda(void *descr[], void *_args){
      GTask *t= (GTask *)_args;
      int n=t->args->args.size()-1;      
      if(debug)cout << "SPUTask is ready to run on GPU  for " << t->args->args[n]->get_name()<< endl << flush;
      double *pMem = (double *)STARPU_MATRIX_GET_PTR(descr[0]);
      double *xMem = (double *)t->args->args[0]->get_memory();
      t->gpuArgs.push_back(descr[0]);
      t->gpuArgs.push_back(descr[1]);
      t->gpuArgs.push_back(descr[2]);
      if(debug)cout << "SPU Mem = " << pMem << " GTask arg0 mem " << xMem << endl << flush;
      if(debug)cout << "descriptor limimt? descr[3]= " << descr[3] << endl << flush;
#if SHORTCUT == 0
      Dispatcher::ready_for_gpu(_spu,t);
#else
      if ( t )
	t->o->run(t);
      else
	printf("null task *\n");
#endif
      
    }
  };
  /*===============================================================================================*/
  class SPU{
  public:
    static string name;
    static int    level;
    /*-------------------------------------------------------------------------------*/
    SPU(){}
    /*-------------------------------------------------------------------------------*/
    static void Init(){
      int ret=starpu_init(NULL);
      STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
      if(debug)cout << "SPU init\n" << flush;
      starpu_cublas_init();
      if(debug)cout << "SPU cuBLAS init\n" << flush;
    }
    /*-------------------------------------------------------------------------------*/
    static void finalize(){
      if(debug)cout << "SPU wait tasks\n" << flush;
      starpu_task_wait_for_all();
      if(debug)cout << "SPU shut down\n" << flush;
      starpu_shutdown();
    }
    /*-------------------------------------------------------------------------------*/
    static void data_created(GData *d){
      #ifdef DT_INCLUDED
      if ( Dispatcher::is_distributed(d->get_level()) )
	return;
      if ( d->get_level() <2)
	return;
      #endif
      if(debug)cout << "data created at SPU " << d->get_name() << endl << flush;
      SPUData *x = new SPUData (d);
      d->set_guest ( static_cast<void *>(x) );
    }
    /*-------------------------------------------------------------------------------*/
    static void data_set_memory(GData *d,double *mem, int ld){
      if(!mem)
	return;
      #ifdef DT_INCLUDED
      if ( d->get_level() <2)
	return;
      #endif
      SPUData *x = static_cast<SPUData  *>(d->get_guest());
      x->register_mem(mem,ld);
    }
    /*-------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
#     if UTP_DEBUG != 0
      cout << "----\t SPU.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
      utp::Dispatcher::ready(_spu,t);
    }
    /*-------------------------------------------------------------------------------*/
    template<typename T,typename P>
    static inline int submit(Task<T,P>*t){
      if(debug)cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl << flush;
      SPUTask<T,P> *spu_task = new SPUTask<T,P>(t);
      int ret = starpu_task_submit(spu_task->get_spu_task() );
      if (STARPU_UNLIKELY(ret == -ENODEV))
	{
	  FPRINTF(stderr, "No worker may execute this task, ret= %d\n",ret);
	  //	  exit(77);
	}
      return ret;
    }
    /*-------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
      if(debug)
	std::cout << "----\t SPU.finished\t" << t->o->name << "_" << t->id << endl;
      utp::Dispatcher::finished(_spu,t);
    }
    /*-------------------------------------------------------------------------------*/
    static void data_partitioned(GData *d ){}
    static void partition_defined(GPartitioner *p){}
    static void partition_cascaded(GPartitioner *p1,GPartitioner *p2){}
    /*-------------------------------------------------------------------------------*/
  };
} 
#endif // SCH_SPU_HPP
