#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <vector>
#include <string>
#include <map>
#include "utp_basic.hpp"
#include "GData.hpp"
#include "GTask.hpp"
#include "GOperation.hpp"
#include "GPartitioner.hpp"
#include "memory_manager.hpp"
#include "config.hpp"
#ifdef LOCAL_DEV
    #include "basic.hpp"
#else
    #ifndef UTP_MQ
       #include "SGWrapper.hpp"
    #endif

#endif

class Tree {
public:
  Tree();
    Tree *find(IScheduler *f);
    Tree *find(int);

    vector<Tree *> next;
    Tree *previous;
    IScheduler *s;
    void *lib_handle;
    string libfile;
};
class Dispatcher
{
    public:
        Dispatcher(int argc, char **argv);
        virtual ~Dispatcher();
        Dispatcher(const Dispatcher& other);
        Dispatcher& operator=(const Dispatcher& other);
        GTask *submit_task(GOperation *,Args *,Axs&,GTask *);
        IScheduler *get_scheduler(int i);
        void run_task(GTask*);
        void dispatch_next(GTask * t);
        void finished_task(GTask *);
        void register_operation(GOperation *, string f);
        void show_oper();
        void finalize();
        IScheduler *get_next_scheduler ( IScheduler *);
        void allocate_memory(GData *);
        void data_partitioned(GData *);
        void data_created(GData *);
        void partition_defined(GPartitioner*);
        void initialize();
        IScheduler * load(int no,string s,string lib);
        void get_thread_info(int &,int&);
        void partition_cascaded(GPartitioner*p1, GPartitioner *p2);
  void set_mq_mode(bool);
    protected:
    private:
        Tree *chain;
        MemoryManager *memMngr;
        int task_cnt,argc;
        static ulong last_scheduler_id;
        char **argv;
        bool mq_mode;
        IScheduler *add_scheduler(const char *);
        void destroy_scheduler(Tree *);
        void data_created(GData *,Tree*);
        void data_partitioned(GData *,Tree*);
        void partition_defined(GPartitioner*,Tree *);
        void partition_cascaded(GPartitioner*, GPartitioner *, Tree *);
};

typedef Dispatcher UnifiedTaskPara;

extern UnifiedTaskPara UTP;
extern Dispatcher *glbDispatcher;
extern map<string,GOperation*> oper_obj;

Dispatcher *get_dispatcher();
void utp_initialize(int, char *[]);
void utp_finalize();

#endif // DISPATCHER_H
