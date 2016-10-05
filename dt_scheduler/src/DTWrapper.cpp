#include "DTWrapper.hpp" 
#define TL LOG_INFO(LOG_MLEVEL,".\n")
/*=======================================================================*/
DTWrapper::DTWrapper(int _id):IScheduler(_id)
{
  LastFuncKey =0;
  ctx = (IContext *)new GenAlgorithm(nullptr);
  pthread_mutexattr_init(&mutex_attr);
  pthread_mutexattr_settype(&mutex_attr,PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&thread_lock,&mutex_attr);
}

/*=======================================================================*/
DTWrapper::~DTWrapper()
{
  pthread_mutex_destroy(&thread_lock);
  pthread_mutexattr_destroy(&mutex_attr);
}

/*=======================================================================*/
DTWrapper::DTWrapper(const DTWrapper& other):IScheduler(-1)
{
  //copy ctor
}

/*=======================================================================*/
DTWrapper& DTWrapper::operator=(const DTWrapper& rhs)
{
  if (this == &rhs) return *this; // handle self assignment
  //assignment operator
  return *this;
}
/*=======================================================================*/
void DTWrapper::submitTask(GTask *t)
{
  LOG_INFO(LOG_MLEVEL,"%s\n",t->fname.c_str());
  if ( t == nullptr){
    return;
  }
  glbCtx.incrementCounter(GlobalContext::TaskRead);

  IData *d=nullptr,*dwrt=nullptr;
  list<DataAccess *> *dlist = new list <DataAccess *>;
  DataAccess *daxs ;
  assert(t->args);
  for ( uint i=0; i < t->args->args.size(); i++){
    GHandle *gh = t->args->args[i]->getHandle();
    assert(gh);
    //d=g2dt_map[gh->get_key()];
    d = (IData *)t->args->args[i]->get_guest();
    assert(d);
      printf("%s,%d\n",__FILE__,__LINE__);
      //    LOG_INFO(LOG_MLEVEL,"ghandle:%ld, dt data p:%s\n",gh->get_key(),d->get_name().c_str());
      printf("%s,%d\n",__FILE__,__LINE__);
    daxs = new DataAccess;
    daxs->data = d;
      printf("%s,%d\n",__FILE__,__LINE__);
      int h = -1;//d->getHost();
      printf("%s,%d\n",__FILE__,__LINE__);
    if ( t->args->args[i]->axs == In){
      daxs->type = IData::READ;
      daxs->required_version = d->getWriteVersion();
      d->incrementVersion(IData::READ);
      LOG_INFO(LOG_MLEVEL,"READ  %s@%d-ver:%s\n",d->get_name().c_str(),h,
	       d->getRunTimeVersion(IData::READ).dumpString().c_str());
    }
    else{
      dwrt = d;
      printf("%s,%d\n",__FILE__,__LINE__);
      daxs->type = IData::WRITE;
      printf("%s,%d\n",__FILE__,__LINE__);
      daxs->required_version = d->getReadVersion();
      printf("%s,%d\n",__FILE__,__LINE__);
      d->incrementVersion(IData::WRITE);
      LOG_INFO(LOG_MLEVEL,"WRITE %s@%d-ver:%s\n",d->get_name().c_str(),h,
	       d->getRunTimeVersion(IData::WRITE).dumpString().c_str());
    }
      printf("%s,%d\n",__FILE__,__LINE__);
    daxs->required_version.setContext( glbCtx.getLevelString() );
      printf("%s,%d\n",__FILE__,__LINE__);
    d->getWriteVersion().setContext( glbCtx.getLevelString() );
      printf("%s,%d\n",__FILE__,__LINE__);
    d->getReadVersion().setContext( glbCtx.getLevelString() );
      printf("%s,%d\n",__FILE__,__LINE__);
    dlist->push_back(daxs);
      printf("%s,%d\n",__FILE__,__LINE__);

  }
      printf("%s,%d\n",__FILE__,__LINE__);
  if (d==nullptr){
      printf("%s,%d\n",__FILE__,__LINE__);
    fprintf(stderr,"No data argument is defined for task.\n ");
    return ;
  }
      printf("%s,%d\n",__FILE__,__LINE__);
  if (dwrt == nullptr){ // if all data are READ
      printf("%s,%d\n",__FILE__,__LINE__);
    dwrt = d;
  }
      printf("%s,%d\n",__FILE__,__LINE__);
  if ( dwrt->getHost()==me){
    LOG_INFO(LOG_MLEVEL,"task added.\n");
    glbCtx.incrementCounter(GlobalContext::TaskInsert);
    TaskHandle task_handle =dtEngine.addTask(ctx,t->fname,KeyGen(t->fname.c_str()),dwrt->getHost(),dlist);

    IDuctteipTask *dt=dtEngine.getTask(task_handle);
    assert(dt);
    //gt2dt_map[t->get_handle()->get_key()]=dt;//Engine.getTask(task_handle);
    //assert( gt2dt_map[t->get_handle()->get_key()]==dtEngine.getTask(task_handle));

    //dt2gt_map[task_handle]=t;
    dt->set_guest((void*)t );
     t->set_guest((void*)dt);
     //assert( dt2gt_map[task_handle]==t);
  }

}
/*=======================================================================*/
void DTWrapper::runTask(GTask *t)
{
  LOG_INFO(LOG_MLEVEL,"%s\n",t->fname.c_str());
}
/*=======================================================================*/
void DTWrapper::finishedTask(GTask *t)
{
  LOG_INFO(LOG_MLEVEL,"%s\n",t->fname.c_str());
  if ( t->get_owner()->get_id() == get_id() ) {
    LOG_INFO(LOG_MLEVEL,"Task is mine.\n");
    IDuctteipTask *tt=(IDuctteipTask *)t->get_guest();//gt2dt_map[t->get_handle()->get_key()];
    if (tt)
      tt->setFinished(true);
    else
      LOG_INFO(LOG_MLEVEL,"Task is not mapped.\n");
    return;
  }
  GTask *parent_task = t->get_parent();
  if ( parent_task == nullptr ) 
    return;
  LOG_INFO(LOG_MLEVEL,"parent Task is %p.\n",parent_task);
  IScheduler *parent_owner = parent_task->get_owner();
  if ( parent_owner == nullptr )
    return;
  LOG_INFO(LOG_MLEVEL,"parent scheduler is %p.\n",parent_owner->get_name().c_str());
  if ( parent_owner->get_id() != get_id() )
    return;
  LOG_INFO(LOG_MLEVEL,"parent Task children cnt :%d.\n",parent_task->task_count);
  

  //int task_cnt = Atomic::decrease_nv(&parent_task->task_count);
  LOG_INFO(LOG_MLEVEL,"parent Task children cnt :%d.\n",parent_task->task_count);
  //  if (parent_task->task_count.load() < 2) {
  pthread_mutex_lock(&thread_lock);
  parent_task->task_count--;
  int task_cnt = parent_task->task_count ;
    if ( task_cnt ==0){
      LOG_INFO(LOG_MLEVEL,"parent Task children cnt :%d.\n",parent_task->task_count);
      IDuctteipTask *tt=(IDuctteipTask *)parent_task->get_guest();//gt2dt_map[parent_task->get_handle()->get_key()];
      if ( tt){
	LOG_INFO(LOG_MLEVEL,"parent Task children cnt :%d task-name:%s.\n",parent_task->task_count,tt->get_name().c_str());
	if ( !tt->isFinished() )
	  tt->setFinished(true);
      }
    }
    pthread_mutex_unlock(&thread_lock);
    //}

}
/*=======================================================================*/
void DTWrapper::data_created(GData *d)
{
  LOG_INFO(LOG_MLEVEL,"data:%s level:%d\n",d->get_name().c_str(),d->get_level());
  if  ( d->get_level() > 1 ) 
    return;
  //GData *dp = d->get_parent();
  //if (dp)     return;
  LOG_INFO(LOG_MLEVEL,"child data:%s\n",d->get_name().c_str());
  //  GHandle *gh = d->getHandle();
  int M = d->get_rows();
  int N = d->get_cols();
  LOG_INFO(LOG_MLEVEL,"M:%d,N:%d\n",M,N);
  OneLevelData *dL1=new OneLevelData(d->get_name(),M,N,ctx);
  //g2dt_map[gh->get_key()] = (IData *)dL1;
    d->set_guest((void*)dL1);
  dL1->set_guest((void*)d  );
}
/*=======================================================================*/
void DTWrapper::data_partitioned(GData *d)
{
  int by = d->get_part_countY();
  int bx = d->get_part_countX();
  LOG_INFO(0*LOG_MLEVEL,"data:%s %dx%d, level:%d\n",d->get_name().c_str(),by,bx,d->get_level());  
  if  ( d->get_level() > 0 ) 
    return;
  //  GHandle *gh = d->getHandle();
  IData *dt ;//= g2dt_map[gh->get_key()];
  dt = (IData *)d->get_guest();
  dt->setPartition(by,bx);
  for ( int i =0 ; i < by; i++){
    for ( int j =0 ; j < bx; j++){
      IData * dt_ch = (*dt)(i,j);
      GData &d_ch = (*d)(i,j);
      assert( dt_ch);
      LOG_INFO(LOG_MLEVEL,"gd_ch:%s, dt_ch:%s\n",d_ch.get_name().c_str(),dt_ch->get_name().c_str());
      d_ch.set_memory((void *)dt_ch->getContentAddress());
      LOG_INFO(LOG_MLEVEL,"gdata memory:%p dt_ch memory:%p\n",d_ch.get_memory(),dt_ch->getContentAddress());
      dt_ch->setParentData(dt);
        d_ch.set_guest((void*) dt_ch);
      dt_ch->set_guest((void*)& d_ch);
      //g2dt_map[d_ch.get_handle()->get_key()]=dt_ch;
    }
  }

}
/*=======================================================================*/
void DTWrapper::finalize()
{
  dtEngine.finalize();
}
/*=======================================================================*/
int DTWrapper::init()
{
  return 0;
}
/*=======================================================================*/
void DTWrapper::allocate_memory(GData *d)
{
  return;//obsolete function
  if (d->get_parent() == nullptr ) 
    return;
  if ( d->is_memory_allocated() ) 
    return;
  int row,col,depth;
  assert(d);
  d->getCoordination(row,col,depth);
  LOG_INFO(LOG_MLEVEL,"Data:%s block-coord(%d,%d)\n",d->get_name().c_str(),row,col);
  //GHandle *gh = d->getHandle();
  OneLevelData *dL1=new OneLevelData(d->get_name(),1,1,ctx);
  IData *dt = (*dL1)(0,0);
  dL1->setBlockIdx(row,col);
  //g2dt_map[gh->get_key()] = (IData *)dL1;
  
  int z= d->get_element_count() * sizeof(double);
  ctx->addInputData((IData*)dL1);
  if ( dL1->getHost() == me ) {
    LOG_INFO(LOG_MLEVEL,"I am the owner, and allocate size:%d\n",z);
    dt->setRunTimeVersion("0.",0);
    dL1->setDataMemory(new MemoryItem(z) ) ;
    //d->set_memory(dL1->getDataMemory());
  }
  else{
    LOG_INFO(LOG_MLEVEL,"I am not the owner, and not allocate\n");
    dt->setRunTimeVersion("-1.",-1);
    dt->setDataMemory(nullptr) ;
    //d->set_memory(nullptr);
  }
}
/*=======================================================================*/
/*----------------------------------------------------------------------------*/
void initData(GData &A){
  //GHandle *gh=A.get_handle();
  IData *dt,*d;
  //  dt=g2dt_map[gh->get_key()];
  dt = (IData *)A.get_guest();
  d = (*dt)(0,0);  
  d->setRunTimeVersion("0.",0);
  dtEngine.putWorkForSingleDataReady(d);
}
/*----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
int LastFuncKey  ;
long  KeyGen(const char *s){//todo : unique based on 's'
  return LastFuncKey ++;
}
map<TaskHandle,GTask *> dt2gt_map;
map<ulong,IDuctteipTask *> gt2dt_map;
map<ulong,dt_data_t*> g2dt_map;




extern "C" {
  IScheduler *f_create(int id)
  {
    return new DTWrapper(id);
  }
  void f_destroy(IScheduler *s)
  {
    delete s;
  }
}
/***************************************************************************************/
/*=======================================================================*/
OneLevelData::OneLevelData(string name,int m, int n,IContext *ctx):IData(name,m,n,ctx)
{
  IData::parent_data = NULL;
  if ( getParent())
    setDataHandle( getParent()->createDataHandle());
  setDataHostPolicy( glbCtx.getDataHostPolicy() ) ;
  setLocalNumBlocks(1,1);
  ctx->addInputData(this);
}
/*=======================================================================*/
int OneLevelData::getHost(){

    return IData::getHost();
    /*
    if (hpData)
      return  hpData->getHost(blk);
    return -1;
    */
  }
/*=======================================================================*/
void GenAlgorithm::runKernels(IDuctteipTask *task) {
    LOG_INFO(LOG_MLEVEL,"%s,handle:%ld\n",task->getName().c_str(),(long)task->getHandle());
    GTask *t;//=dt2gt_map[task->getHandle()];
    t = (GTask*)task->get_guest();
    assert(get_dispatcher());
    LOG_INFO(LOG_MLEVEL,"Gtask :%s\n",t->get_name().c_str());
    get_dispatcher()->run_task(t);

  }
/*=======================================================================*/
void DTWrapper::partition_cascaded(GPartitioner *p1, GPartitioner *p2)
{
}
void DTWrapper::partition_defined(GPartitioner *p)
{
}
