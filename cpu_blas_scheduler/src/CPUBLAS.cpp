#include "CPUBLAS.hpp"
#include <cstdio>
#include "unified_blas.hpp"
#include "task_blas.hpp"
/*==============================================================*/
CPUBLAS::CPUBLAS(int i):IScheduler(i)
{
    //ctor
}

/*==============================================================*/
int CPUBLAS::init()
{
    return 0;
}
/*==============================================================*/
CPUBLAS::~CPUBLAS()
{
    //dtor
}

/*==============================================================*/
CPUBLAS::CPUBLAS(const CPUBLAS& other):IScheduler(-1)
{
    //copy ctor
}

/*==============================================================*/
CPUBLAS& CPUBLAS::operator=(const CPUBLAS& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

/*==============================================================*/
void CPUBLAS::submitTask(GTask *t)
{
    if ( t== nullptr)
    {
        return;
    }

}
/*==============================================================*/
void CPUBLAS::runTask(GTask *t)
{
  LOG_INFO(LOG_MLEVEL,"task run %s\n",t->get_name().c_str());
    blas_map(gemm);  
    blas_map(trsm);
    blas_map(dot);
    blas_map(add);
    blas_map(copy);
    Dispatcher *d = get_dispatcher();
    assert(d);
    d->finished_task(t);
    //printf("%s,%d,%s\n",__FILE__,__LINE__,__FUNCTION__);

}
/*==============================================================*/

extern "C" {
    IScheduler *f_create(int id)
    {
        return new CPUBLAS(id);
    }
    void f_destroy(IScheduler *s)
    {
        delete s;
    }
}
/*==============================================================*/
void CPUBLAS::finishedTask(GTask *t)
{
}
void CPUBLAS::finalize()
{
}
void CPUBLAS::allocate_memory(GData *d)
{
}
void CPUBLAS::data_created(GData *)
{
}
void CPUBLAS::data_partitioned(GData *)
{
}
/*==============================================================*/
