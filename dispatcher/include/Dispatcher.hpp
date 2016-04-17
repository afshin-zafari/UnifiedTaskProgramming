#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <vector>
#include <string>
#include <map>
#include "utp_basic.hpp"
#include "GData.hpp"
#include "GTask.hpp"
#include "GOperation.hpp"
#include "memory_manager.hpp"
#include "SGWrapper.hpp"

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
        GTask *submit_task(string,Args *,Axs&,GTask *);
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
        void initialize();
  IScheduler * load(int no,string s,string lib);
    protected:
    private:
        Tree *chain;
        MemoryManager *memMngr;
        int task_cnt,argc;
        static ulong last_scheduler_id;
        char **argv;
        IScheduler *add_scheduler(const char *);
        void destroy_scheduler(Tree *);
        void data_created(GData *,Tree*);
        void data_partitioned(GData *,Tree*);
};

typedef Dispatcher UnifiedTaskPara;

extern UnifiedTaskPara UTP;
extern Dispatcher *glbDispatcher;
extern map<string,GOperation*> oper_obj;

Dispatcher *get_dispatcher();
void utp_initialize(int, char *[]);
void utp_finalize();

#endif // DISPATCHER_H
