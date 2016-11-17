#ifndef SCH_DUCTTEIP_HPP
#define SCH_DUCTTEIP_HPP
#include <iostream>
#include "operation.hpp"
#include "dispatcher.hpp"
#include "ductteip.hpp"
using namespace std;
namespace utp{
    class DT;
    extern DT _dt;
    typedef IData dt_data_t;
/*=================================================================================*/
class OneLevelData:public IData{
private:
public:
  OneLevelData(string name,int m,int n,IContext *ctx):IData(name,m,n,ctx)
  {
      IData::parent_data = NULL;
      if ( getParent())
        setDataHandle( getParent()->createDataHandle());
      setDataHostPolicy( glbCtx.getDataHostPolicy() ) ;
      setLocalNumBlocks(1,1);
      ctx->addInputData(this);
  }
  //int getHost();
};

/*=================================================================================*/

class GenAlgorithm : public Algorithm{
private:
  string name;
public :
  GenAlgorithm(){
     name = "DTContext";
  }
  void taskFinished(IDuctteipTask *task, ::TimeUnit dur){
    int i = 0 ;
    (void ) i;
  }
  void runKernels(IDuctteipTask *task){}
  string getTaskName(unsigned long) {return name;}

};
GenAlgorithm *dt_ctx;

/*=================================================================================*/
//template <typename T>void GenAddDTTask( utp::Task<OperationBase<T>>  *t);
template <typename T,typename P>
class DTTask : public IDuctteipTask
{
public:
  typedef  utp::Task<T,P> GTask;
    GTask * gtask;
    DTTask(GTask *t):gtask(t)
    {
        gtask->guest = (void*)this;
#     if DEBUG != 0 
        cout << "----\t  DT.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
        assert(t);
        glbCtx.incrementCounter(GlobalContext::TaskRead);

        IData *d=nullptr,*dwrt=nullptr;
        list<DataAccess *> *dlist = new list <DataAccess *>;
        DataAccess *daxs ;
        assert(t->args);
        for ( uint i=0; i < t->args->args.size(); i++)
        {
            GHandle *gh = t->args->args[i]->getHandle();
            assert(gh);
            d = (IData *)t->args->args[i]->get_guest();
            assert(d);
            daxs = new DataAccess;
            daxs->data = d;
            int h = -1;
            if ( t->axs->axs[i]  == In){
              daxs->type = IData::READ;
              daxs->required_version = d->getWriteVersion();
              d->incrementVersion(IData::READ);
              PRINTF("READ  %s@%d-ver:%s\n",d->get_name().c_str(),h,
                   d->getRunTimeVersion(IData::READ).dumpString().c_str());
            }
            else{
              dwrt = d;
              daxs->type = IData::WRITE;
              daxs->required_version = d->getReadVersion();
              d->incrementVersion(IData::WRITE);
              PRINTF("WRITE %s@%d-ver:%s\n",d->get_name().c_str(),h,
                   d->getRunTimeVersion(IData::WRITE).dumpString().c_str());
            }
            daxs->required_version.setContext( glbCtx.getLevelString() );
            d->getWriteVersion().setContext( glbCtx.getLevelString() );
            d->getReadVersion().setContext( glbCtx.getLevelString() );
            dlist->push_back(daxs);
        }
        if (d==nullptr){
            fprintf(stderr,"No data argument is defined for task.\n ");
            return ;
        }
        if (dwrt == nullptr){ // if all data are READ
            dwrt = d;
        }
        if ( dwrt->getHost()==me){
            PRINTF("task added.\n");
	    setDataAccessList(dlist);
	    setHost(me);
            glbCtx.incrementCounter(GlobalContext::TaskInsert);
            dtEngine.register_task(this);
        }
    }
    void run(){
      if ( isFinished())
	return;
      if ( getState() == Running )
	return;
      setState( Running);
      Dispatcher::ready(_dt,gtask);
    }
};
/*=================================================================================*/
class DT{
public:

    pthread_mutex_t 	thread_lock;
    pthread_mutexattr_t 	mutex_attr;
    string name ;
    DT():name("DT")
    {
        dt_ctx = nullptr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr,PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&thread_lock,&mutex_attr);
    }
    ~DT()
    {
        pthread_mutex_destroy(&thread_lock);
        pthread_mutexattr_destroy(&mutex_attr);
    }
    static void finalize()
    {
        dtEngine.finalize();
    }
    static void init(){
        dt_ctx = new GenAlgorithm;
    }
    /*------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static int submit(Task<T,P> *t){
      DTTask<T,P> *temp = new DTTask<T,P> (t);
      assert(temp);
      return 1;
    }
    template <typename T,typename P>
    static void finishedTask(Task<T,P>  *t)
    {
      DTTask<T,P>* dt =static_cast<DTTask<T,P>*>( t->guest);
        dt->setFinished(true);
    }
    /*------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
#     if DEBUG != 0 
        cout << "----\t  DT.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
        utp::Dispatcher::ready(_dt,t);
    }
    /*------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
#     if DEBUG != 0 
        std::cout << "----\t DT.finished\t" << t->o->name << "_" << t->id << endl;
#     endif
        finishedTask(t);
    }
    /*------------------------------------------------------------------------------*/
    static void data_created(GData *d)
    {
      PRINTF("data:%s level:%d\n",d->get_name().c_str(),d->get_level());
      if  ( d->get_level() > 1 )
        return;


      PRINTF("child data:%s\n",d->get_name().c_str());

      int M = d->get_rows();
      int N = d->get_cols();
      PRINTF("M:%d,N:%d\n",M,N);

      OneLevelData *dL1=new OneLevelData(d->get_name(),M,N,dt_ctx);

        d->set_guest((void*)dL1);
      dL1->set_guest((void*)d  );
    }
    /*------------------------------------------------------------------------------*/
    static void data_partitioned(GData *d)
    {
      int by = d->get_part_countY();
      int bx = d->get_part_countX();
      PRINTF("data:%s %dx%d, level:%d\n",d->get_name().c_str(),by,bx,d->get_level());
      if  ( d->get_level() > 0 )
        return;

      IData *dt ;
      dt = (IData *)d->get_guest();
      dt->setPartition(by,bx);
      for ( int i =0 ; i < by; i++){
        for ( int j =0 ; j < bx; j++){
          IData * dt_ch = (*dt)(i,j);
          GData &d_ch = (*d)(i,j);
          assert(dt_ch);
          PRINTF("gd_ch:%s, dt_ch:%s\n",d_ch.get_name().c_str(),dt_ch->get_name().c_str());
          d_ch.set_memory((void *)dt_ch->getContentAddress());
          PRINTF("gdata memory:%p dt_ch memory:%p\n",d_ch.get_memory(),dt_ch->getContentAddress());
          dt_ch->setParentData(dt);
            d_ch.set_guest((void*) dt_ch);
          dt_ch->set_guest((void*)& d_ch);
        }
      }
    }
    /*-------------------------------------------------*/
    static void partition_cascaded(GPartitioner *p1, GPartitioner *p2){}
    static void partition_defined(GPartitioner *p){}
    /*-------------------------------------------------*/



};

class DT2:public DT{};
}
#endif // SCH_DUCTTEIP_HPP
