#include "Dispatcher.hpp"
#include "SGWrapper.hpp"
#include "DTWrapper.hpp"
#include "CPUBLAS.hpp"
#include "CUBLAS.hpp"
#include <string>
#include <iostream>
#include <dlfcn.h>

using namespace std;

void packArgs(Args *a) {}
void packAxs (Axs  &a) {}
const int In   = 1;
const int Out  = 2;
const int InOut= 3;
ulong Dispatcher::last_scheduler_id=0;
map<string,GOperation*> oper_obj;
Dispatcher *glbDispatcher;

/*=============================================================*/
Tree::Tree()
{
  s=nullptr;
  previous = nullptr;
}
/*=============================================================*/

Tree *Tree::find(IScheduler *f)
{
    if ( s->get_id() == f->get_id())
        return this;
    for ( Tree* t: next)
    {
        Tree * fnd=t->find(f);
        if (fnd) return fnd;
    }
    return nullptr;
}
Tree *Tree::find(int i)
{
    if ( s == nullptr)
        return nullptr;
    if ( s->get_id() == (uint)i)
        return this;
    for ( Tree* t: next)
    {
        Tree * fnd=t->find(i);
        if (fnd) return fnd;
    }
    return nullptr;
}
/*=============================================================*/
typedef IScheduler * (*fp_create)(int);
typedef void (*fp_destroy)(IScheduler *);


/*=============================================================*/
IScheduler *Dispatcher::add_scheduler(const char *s)
{
    char *error;
    Tree *next,*last =chain;
    LOG_INFO(LOG_MLEVEL,"Load a dynamic libray '%s'.\n",s);
    if ( last != nullptr)
    {
      LOG_INFO(LOG_MLEVEL,"not first sh.lib.\n");
        while ( last->next.size() !=0)
            last = last->next[0];
        next=new Tree;
        last->next.push_back(next);
        next->previous = last;
    }
    else
    {
      LOG_INFO(LOG_MLEVEL,"1st sh.lib.\n");
        chain =new Tree;
        next = chain;
    }
    LOG_INFO(LOG_MLEVEL,"Load shared libray:%s.\n",s);
    next->lib_handle = dlopen(s, RTLD_GLOBAL | RTLD_LAZY);
    if (!next->lib_handle)
    {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    LOG_INFO(LOG_MLEVEL,"Call the f_create of the sh.lib.\n");
    fp_create fn = (fp_create)dlsym(next->lib_handle, "f_create");
    if ((error = dlerror()) != NULL)
    {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
    next->s =fn(last_scheduler_id++);
    next->s->submitTask((GTask *) NULL);
    return next->s;
}
/*=============================================================*/
Dispatcher::Dispatcher(int argc_, char **argv_)
{
  chain = nullptr;
  argc = argc_;
  argv = argv_;

}
/*=============================================================*/
void Dispatcher::destroy_scheduler(Tree *c)
{
    fp_destroy dtor = (fp_destroy)dlsym(c->lib_handle, "f_destroy");
    dtor(c->s);
    dlclose(c->lib_handle);
    for ( Tree *n : c->next)
        destroy_scheduler(n);
}
/*=============================================================*/
Dispatcher::~Dispatcher()
{
  //destroy_scheduler(chain);
  LOG_INFO(LOG_MLEVEL,"Total Tasks:%d\n",task_cnt);
}
/*=============================================================*/
Dispatcher::Dispatcher(const Dispatcher& other)
{
    //copy ctor
}
/*=============================================================*/
Dispatcher& Dispatcher::operator=(const Dispatcher& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*=============================================================*/
GTask *Dispatcher::submit_task(string fn, Args *args, Axs & axs,GTask *parent_task)
{
    for ( uint i=0; i < args->args.size(); i++)
    {
        if ( args->args[i] != nullptr )
            args->args[i]->axs = axs.axs[i];
    }

    GTask *t=new GTask(fn,args,-1);
    t->set_parent(parent_task);
    dispatch_next(t);
    task_cnt++;
    return t;
}
/*=============================================================*/
IScheduler *Dispatcher::get_scheduler( int i )
{
    Tree *t=chain->find(i);
    if (t != nullptr)
        return t->s;
    return nullptr;
}
/*=============================================================*/
void Dispatcher::dispatch_next(  GTask * t)
{
    GTask *parent = t->get_parent();
    if ( parent == nullptr)
    {
        t->set_owner(chain->s);
	assert(chain);
	assert(chain->s);
        chain->s->submitTask(t);
        return;
    }
    IScheduler *cur = parent->get_owner();
    Tree * pos = chain->find(cur);
    assert(pos!=nullptr);
    for (  Tree *node : pos->next)
    {
        t->set_owner(node->s);
        node->s->submitTask(t);
    }
}
/*=============================================================*/
void Dispatcher::run_task(GTask *t)
{
    IScheduler *s = t->get_owner();
    Tree *c=chain->find(s);
    assert(c!=nullptr);
    Tree *n = c->next[0];
    if(n->next.size()==0)// it is before last Scheduler
    {
        n->s->runTask(t);
        return;
    }
    GOperation *o = oper_obj[t->fname];
    if ( o )
        o->run(t);
}
/*=============================================================*/
void Dispatcher::finished_task(GTask *t)
{
    //    LOG_INFO(LOG_MLEVEL,"task finished :%s\n",t->fname.c_str());
    Tree *sch=chain->find(t->get_owner());
    assert(sch != nullptr);
    IScheduler *prev;
    assert(sch->s);
    LOG_INFO(LOG_MLEVEL,"owner  scheduler:%s\n",sch->s->get_name().c_str());
    Tree *n = sch->next[0];    
    if(n->next.size()==0)// it is before last Scheduler
      {
	LOG_INFO(LOG_MLEVEL,"owner scheduler:%s\n",sch->s->get_name().c_str());
	sch->s->finishedTask(t);
	if ( sch->previous == nullptr)
	  return;
	if ( sch->previous->s )
	  LOG_INFO(LOG_MLEVEL,"previous  scheduler:%s\n",sch->previous->s->get_name().c_str());
      }
    
    if ( sch->previous == nullptr)
        prev  = sch->s;
    else
        prev=sch->previous->s;
    assert(prev);
    LOG_INFO(LOG_MLEVEL,"prev scheduler:%s\n",prev->get_name().c_str());
    prev->finishedTask(t);

}
/*=============================================================*/
Dispatcher *get_dispatcher()
{
    return glbDispatcher;
}
/*=============================================================*/
void Dispatcher::register_operation(GOperation *op,string f)
{
    oper_obj[f]=op;
}
/*=============================================================*/
void Dispatcher::show_oper()
{
    cout << "oper1: " << oper_obj.begin()->first << endl;
    cout << "oper1: " << oper_obj.begin()->second<< endl;
}
/*=============================================================*/
void Dispatcher::finalize()
{
    chain->s->finalize();
    LOG_INFO(LOG_MLEVEL,"First scheduler:%s\n",chain->s->get_name().c_str());
    //    if ( chain->s->get_name() != "DuctTeip" )
    int flag;
    MPI_Finalized(&flag);
    if (!flag)
      MPI_Finalize();
    LOG_INFO(LOG_MLEVEL,"First scheduler:%s\n",chain->s->get_name().c_str());
}
/*=============================================================*/
IScheduler *Dispatcher::get_next_scheduler(IScheduler *sch)
{
    Tree *c = chain->find(sch);
    if ( !c)
        return nullptr;
    if ( c->next.size() ==0)
        return nullptr;
    return c->next[0]->s;
}
/*=============================================================*/
void Dispatcher::allocate_memory(GData *d)
{
  return;
}
/*=============================================================*/
void Dispatcher::data_created(GData *d, Tree *ch)
{
  ch->s->data_created(d);
  for (uint i=0;i< ch->next.size();i++ ){
    data_created(d,ch->next[i]);
  }
}
/*=============================================================*/
void Dispatcher::data_created(GData *d)
{
  assert(chain);
  data_created(d,chain);
}
/*=============================================================*/
void Dispatcher::data_partitioned(GData *d, Tree *ch)
{
  ch->s->data_partitioned(d);
  for (uint i=0;i< ch->next.size();i++ ){
    data_partitioned(d,ch->next[i]);
  }
}
/*=============================================================*/
void Dispatcher::data_partitioned(GData *d)
{
  assert(chain);
  data_partitioned(d,chain);
}
/*=============================================================*/
IScheduler * Dispatcher::load(int no,string s,string lib)
{
  IScheduler *sch=nullptr;

  if ( s == "DuctTeip"){
    LOG_INFO(LOG_MLEVEL,"The Scheduler No.:%d is DuctTeip.\n",no);
    sch= new DTWrapper(last_scheduler_id++);
    sch->set_name("DuctTeip");
    dtEngine.start(argc,argv);
  }
  if(s == "SuperGlue"){
    LOG_INFO(LOG_MLEVEL,"The Scheduler No.:%d is SuperGlue.\n",no);
    sch= new SGWrapper(last_scheduler_id++);
    sch->set_name("SuperGlue");
  }
  if(s == "cpuBLAS"){
    LOG_INFO(LOG_MLEVEL,"The Scheduler No.:%d is cpuBLAS.\n",no);
    sch = new CPUBLAS(last_scheduler_id++);
    sch->set_name("cpuBLAS");
  }
  /*
  if(s == "cuBLAS"){
    LOG_INFO(LOG_MLEVEL,"The Scheduler No.:%d is cuBLAS.\n",no);
    sch = new CUBLAS(last_scheduler_id++);
    sch->set_name("cuBLAS");
  }
  */
  if ( sch == nullptr ){
    fprintf(stderr,"The scheduler No.:%d (%s) could not be loaded.\n",no,(char *)s.c_str());
    exit(-2);
  }
  return sch;
}
/*=============================================================*/
void Dispatcher::initialize()
{
  Tree *last;
  if ( config.sch1.size() ==0){
      fprintf(stderr,"No Library for first scheduler is given.\n");
      exit(-1);
  }
  chain = new Tree;
  chain->s = load(1,config.sch1,config.lib1);
  last = chain;
  if (config.sch2.size() !=0){
    Tree *n = new Tree;
    n->s = load(2,config.sch2,config.lib2);
    last->next.push_back(n);
    n->previous = last;
    last = n;
  }
  if (config.sch3.size() !=0){
    Tree *n = new Tree;
    n->s = load(3,config.sch3,config.lib3);
    last->next.push_back(n);
    n->previous = last;
  }

  task_cnt = 0;
}
void Dispatcher::get_thread_info( int & n, int & pin_cpu)
{
  n = config.getNumThreads();
  pin_cpu = me * n;
}
/***************************************************************/
void utp_initialize(int argc, char **argv)
{
  config.getCmdLine(argc,argv);
  glbDispatcher = new Dispatcher(argc,argv);
  glbDispatcher->initialize();
}
/*=============================================================*/
void utp_finalize()
{
  get_dispatcher()->finalize();
}
/*=============================================================*/
