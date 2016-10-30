#include "EchoScheduler.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include "GPartitioner.hpp"
#include <stdlib.h>
#include <stdio.h>
#define LOG(x) printf("%s, %s\n",__PRETTY_FUNCTION__,x)

const int In   = 1;
const int Out  = 2;
const int InOut= 3;

/*=========================================================================*/
EchoScheduler::EchoScheduler(int id):IScheduler(id)
{
}

/*=========================================================================*/
int EchoScheduler::init()
{
    return 0;
}
/*=========================================================================*/
EchoScheduler::~EchoScheduler()
{

}

/*=========================================================================*/
EchoScheduler::EchoScheduler(const EchoScheduler& other,int id):IScheduler(id)
{
    //copy ctor
}

/*=========================================================================*/
EchoScheduler& EchoScheduler::operator=(const EchoScheduler& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*=========================================================================*/
void EchoScheduler::submitTask(GTask *t)
{


}
/*=========================================================================*/
void EchoScheduler::runTask(GTask *t)
{

}
/*=========================================================================*/
void EchoScheduler::finishedTask(GTask *t)
{

}
/*=========================================================================*/
void EchoScheduler::finalize()
{
}
/*=========================================================================*/
void EchoScheduler::allocate_memory(GData*d)
{

}
/*=========================================================================*/
void EchoScheduler::data_created(GData *d)
{
LOG("Data Created");

}
/*=========================================================================*/
void EchoScheduler::data_partitioned(GData *d)
{

}
void EchoScheduler::partition_defined(GPartitioner*p)
{

}

/*=========================================================================*/
extern "C" {
    IScheduler *f_create(int id)
    {
        return new EchoScheduler(id);
    }
    void f_destroy(IScheduler *s)
    {
        delete s;
    }
}
void EchoScheduler::partition_cascaded(GPartitioner*,GPartitioner*)
{
}
