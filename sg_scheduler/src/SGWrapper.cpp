#include "SGWrapper.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include <stdlib.h>

SuperGlue<SGWOptions> *SG;
const int In   = 1;
const int Out  = 2;
const int InOut= 3;

/*=========================================================================*/
SGWrapper::SGWrapper(int id):IScheduler(id)
{
  int num_threads=-1, pin_cpu=0;
  // get_dispatcher()->get_thread_info(num_threads,pin_cpu);
  SG = new SuperGlue<SGWOptions>(num_threads,pin_cpu); 
}

/*=========================================================================*/
int SGWrapper::init()
{
    return 0;
}
/*=========================================================================*/
SGWrapper::~SGWrapper()
{
    //dtor
}

/*=========================================================================*/
SGWrapper::SGWrapper(const SGWrapper& other,int id):IScheduler(id)
{
    //copy ctor
}

/*=========================================================================*/
SGWrapper& SGWrapper::operator=(const SGWrapper& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*=========================================================================*/
void SGWrapper::submitTask(GTask *t)
{
    if ( !t)
    {
        return;
    }
    SGGenTask *sgt = new SGGenTask(this,t);
    SG->submit(sgt);

}
/*=========================================================================*/
void SGWrapper::runTask(GTask *t)
{
}
/*=========================================================================*/
void SGWrapper::finishedTask(GTask *t)
{

}
/*=========================================================================*/
void SGWrapper::finalize()
{
    SG->barrier();
}
/*=========================================================================*/
void SGWrapper::allocate_memory(GData*)
{
}
/*=========================================================================*/
void SGWrapper::data_created(GData *d)
{
  //  GHandleKey gh = d->get_handle()->get_key();
  sg_data_t *sgd= (sg_data_t*)d->get_guest();
  if ( sgd == NULL ) {
    sgd = new sg_data_t;
    d->set_guest((void *)sgd);
  }
  //  if (glb_to_sg[gh]==NULL)    glb_to_sg[gh]=new sg_data_t;
  if ( d->get_level() ==0){
    int size = d->get_cols() * d->get_rows() * sizeof(double);
    void *m =(void *) new byte[size];
    d->set_memory(m);
  }
}
/*=========================================================================*/
void SGWrapper::data_partitioned(GData *d)
{
  int by = d->get_part_countY();
  int bx = d->get_part_countX();
  int m = d->get_rows();
  int n = d->get_cols();
  int child_m = m/by;
  int child_n = n/bx;
  int mn = child_m*child_n;
  if ( mn == 1) 
    return;
  if ( get_id() == 1 && d->get_level() == 0){
    LOG_INFO(LOG_MLEVEL,"Skip this partitioning %dx%d %dx%d at level:%d since my level is:%d.\n",by,bx,m,n,d->get_level(),get_id());
    return;
  }
  double *content = (double *)d->get_memory();
  LOG_INFO(0&LOG_MLEVEL,"parent gdata memory:%p \n",d->get_memory());
  for ( int i =0 ; i < by; i++){
    for ( int j =0 ; j < bx; j++){
      GData &d_ch = (*d)(i,j);
      LOG_INFO(0&LOG_MLEVEL,"gd_ch:%s\n",d_ch.get_name().c_str());
      d_ch.set_memory((void*)(content+(j*by+i)*mn));
      LOG_INFO(0&LOG_MLEVEL,"child (%d,%d) gdata memory:%p \n",i,j,d_ch.get_memory());
    }
  }
  
}
/*=========================================================================*/

/****************************************************************************/
//map<GHandleKey,sg_data_t*> glb_to_sg;
SGGenTask::SGGenTask(SGWrapper *sgw,GTask *t):scheduler(sgw),gt(t)
{
    Args *a=t->args;
    for(uint i=0; i< a->args.size(); i++)
    {
      //GHandleKey gh = a->args[i]->get_handle()->get_key();
	sg_data_t *sgd = (sg_data_t *)a->args[i]->get_guest();
	if ( sgd == NULL ){ 
            sgd =new sg_data_t;
	    a->args[i]->set_guest((void*)sgd);
	}
	//        if (glb_to_sg[gh]==NULL)            glb_to_sg[gh]=new sg_data_t;
        sg_data_t *h = sgd;//glb_to_sg[gh];
        if ( a->args[i]->axs==In)
            register_access(ReadWriteAdd::read,*h);
        else
            register_access(ReadWriteAdd::write,*h);
    }
}
/*=========================================================================*/
void SGGenTask::run()
{
  //  LOG_INFO(LOG_MLEVEL,"\n");
    get_dispatcher()->run_task(gt);
}
/*=========================================================================*/
string SGGenTask::get_name()
{
    return gt->fname;
}
/*=========================================================================*/
SGGenTask::~SGGenTask()
{
}
/*=========================================================================*/
extern "C" {
    IScheduler *f_create(int id)
    {
        return new SGWrapper(id);
    }
    void f_destroy(IScheduler *s)
    {
        delete s;
    }
}
