#ifndef SCH_SPU_HPP
#define SCH_SPU_HPP
#include "operation.hpp"
#include "dispatcher.hpp"
#include "starpu.h"

#define FPRINTF(ofile, fmt, ...) do { if (!getenv("STARPU_SSILENT")) {fprintf(ofile, fmt, ## __VA_ARGS__); }} while(0)


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
      starpu_matrix_data_register(&handle, 0, (uintptr_t)mem, N, N, M, sizeof(ElementType));
      cout << "Data " << x->get_name() << "registered at SPU with handle " << handle << endl;
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
      cl.cpu_funcs[0] =SPUTask::run;
      cl.nbuffers = gtask->args->args.size();
      cout << "SPUTask created with arg count: " << cl.nbuffers << endl;
      for(int i=0;i<cl.nbuffers;i++){
	if      (gtask->axs->axs[i] == In    ) cl.modes[i] =STARPU_R;
	else if (gtask->axs->axs[i] == Out   ) cl.modes[i] =STARPU_W;
	else if (gtask->axs->axs[i] == InOut ) cl.modes[i] =STARPU_RW;
	cout << "access of arg " << i << " = " << cl.modes[i] << endl;
      }
      cl.name = "general_spu_cl";
    }
  /*-----------------------------------------------------------------------------------------*/
    starpu_task *get_spu_task(){
      cout << "get_spu_task called\n";
      starpu_task *spu_task = starpu_task_create();
      spu_task->cl = clp;
      spu_task->cl_arg = (void  *) gtask;
      spu_task->cl_arg_size = sizeof(gtask);

      for(int i=0;i<clp->nbuffers;i++){
	SPUData *a = static_cast<SPUData *>(gtask->args->args[i]->get_guest());
	spu_task->handles[i] = a->handle;
	cout << "handle of spu data" << a->handle << endl;
      }
      return spu_task;
    }
  /*-----------------------------------------------------------------------------------------*/
    static void run(void *descr[], void *_args){
      GTask *t= (GTask *)_args;
      cout << "SPUTask is ready to run for " << t->args->args[2]->get_name()<< endl;
#if SHORTCUT == 0
      Dispatcher::ready(_spu,t);
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
      cout << "SPU init\n";
      STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
    }
    /*-------------------------------------------------------------------------------*/
    static void finalize(){
      starpu_task_wait_for_all();
      starpu_shutdown();
    }
    /*-------------------------------------------------------------------------------*/
    static void data_created(GData *d){
      //      if ( Dispatcher::is_distributed(d->get_level()) )	return;
      SPUData *x = new SPUData (d);
      cout << "data created at SPU " << d->get_name() << endl;
      d->set_guest ( static_cast<void *>(x) );
    }
    /*-------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
#     if DEBUG != 0
      cout << "----\t SPU.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
      utp::Dispatcher::ready(_spu,t);
    }
    /*-------------------------------------------------------------------------------*/
    template<typename T,typename P>
    static inline int submit(Task<T,P>*t){
      cout << "----\t SPU.submit\t" << t->o->name << "_" << t->id << endl;
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
#     if DEBUG != 0
      std::cout << "----\t SPU.finished\t" << t->o->name << "_" << t->id << endl;
#     endif
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
