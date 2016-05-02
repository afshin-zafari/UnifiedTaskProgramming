#ifndef DTWRAPPER_HPP
#define DTWRAPPER_HPP
#include "IScheduler.hpp"
#include "Dispatcher.hpp"
#include "GData.hpp"
#include "GTask.hpp"
#include "ductteip.hpp"

long  KeyGen(const char *s);
extern int LastFuncKey ;

/*=================================================================================*/
class DTWrapper : public IScheduler
{
    public:
        DTWrapper(int);
        ~DTWrapper();
        DTWrapper(const DTWrapper& other);
        DTWrapper& operator=(const DTWrapper& other);
        void submitTask(GTask *);
        void runTask(GTask *);
        void finishedTask(GTask *);
        void finalize();
        int init();
        void allocate_memory(GData *);
        void data_created(GData *);
        void data_partitioned(GData *);
    protected:
    private:
  IContext *ctx;
  pthread_mutex_t 	thread_lock;
  pthread_mutexattr_t 	mutex_attr;

};

typedef IData dt_data_t;
//extern map<TaskHandle,GTask *> dt2gt_map;
//extern map<ulong,IDuctteipTask *> gt2dt_map;
//extern map<ulong,dt_data_t*> g2dt_map;
/*=================================================================================*/
class OneLevelData:public IData{
private:
public:
  OneLevelData(string name,int,int,IContext *ctx);
  int getHost();
};

/*=================================================================================*/
class GenAlgorithm : public Algorithm{
private:
  GTask *gt;
public :
  GenAlgorithm(GTask *t):gt(t){
  }
  void taskFinished(IDuctteipTask *task, TimeUnit dur){
    LOG_INFO(LOG_MLEVEL,"%s, dur:%ld\n",task->getName().c_str(),dur);
  }
  void runKernels(IDuctteipTask *task) {
    LOG_INFO(LOG_MLEVEL,"%s,handle:%ld\n",task->getName().c_str(),(long)task->getHandle());
    GTask *t;//=dt2gt_map[task->getHandle()];
    t = (GTask*)task->get_guest();
    assert(get_dispatcher());
    LOG_INFO(LOG_MLEVEL,"Gtask ptr:%p\n",t);
    get_dispatcher()->run_task(t);

  }
  string getTaskName(unsigned long) {return gt->fname;}

};
void GenAddDTTask( GTask *t);
#endif // DTWRAPPER_HPP

