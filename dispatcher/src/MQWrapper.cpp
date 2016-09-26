#include "MQWrapper.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include <stdlib.h>


template <>
void Queue::put(GData *d,int tag){

    byte *buf = new byte[128];
    int ofs=0;
    d->serialize(buf,ofs);
    Message *msg = new Message ( buf, ofs, tag);
    msg_list.push_back(msg);
}
template <>
void Queue::put(GTask *t,int tag){
    byte *buf=new byte[256];
    int ofs=0;
    t->serialize(buf,ofs);
    Message *msg = new Message ( buf, ofs, tag);
    msg_list.push_back(msg);
}

const int In   = 1;
const int Out  = 2;
const int InOut= 3;

/*=========================================================================*/
MQWrapper::MQWrapper(int id):IScheduler(id)
{
  mq_send = new Queue;
  mq_recv = new Queue;
}

/*=========================================================================*/
int MQWrapper::init()
{
    return 0;
}
/*=========================================================================*/
MQWrapper::~MQWrapper()
{
    delete mq_send;
    delete mq_recv;
}

/*=========================================================================*/
MQWrapper::MQWrapper(const MQWrapper& other,int id):IScheduler(id)
{
    //copy ctor
}

/*=========================================================================*/
MQWrapper& MQWrapper::operator=(const MQWrapper& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*=========================================================================*/
void MQWrapper::submitTask(GTask *t)
{
    if ( !t)
    {
        return;
    }
    mq_send->put(t,MQ_TASK_SUBMIT);

}
/*=========================================================================*/
void MQWrapper::runTask(GTask *t)
{
    mq_send->put(t,MQ_TASK_READY);
}
/*=========================================================================*/
void MQWrapper::finishedTask(GTask *t)
{
    mq_send->put(t,MQ_TASK_FINISHED);
}
/*=========================================================================*/
void MQWrapper::finalize()
{
}
/*=========================================================================*/
void MQWrapper::allocate_memory(GData*d)
{
    mq_send->put(d,MQ_DATA_ALLOCATED);
}
/*=========================================================================*/
void MQWrapper::data_created(GData *d)
{
    mq_send->put(d,MQ_DATA_CREATED);
}
/*=========================================================================*/
void MQWrapper::data_partitioned(GData *d)
{
    mq_send->put(d,MQ_DATA_PARTITIONED);
}
void MQWrapper::run_rpc(){
    Message *msg = mq_recv->get();
    Dispatcher * dis = get_dispatcher();
    GTask *t;
    GData *d;
    GOperation *o;
    int ofs=0;
    switch(msg->tag){
        case MQ_TASK_FINISHED:
            t = DeserializeTask(msg->buf,ofs);
            dis->finished_task(t);
            break;
        case MQ_TASK_READY:
            t = DeserializeTask(msg->buf,ofs);
            dis->run_task(t);
            break;
        case MQ_TASK_SUBMIT:
            t = DeserializeTask(msg->buf,ofs);
            dis->submit_task(t->get_operation(),t->get_args(),*t->get_axs(),t->get_parent());
            break;
        case MQ_DATA_ALLOCATED:
            d = DeserializeData(msg->buf,ofs);
            dis->allocate_memory(d);
            break;
        case MQ_DATA_CREATED:
            d = DeserializeData(msg->buf,ofs);
            dis->data_created(d);
            break;
        case MQ_DATA_PARTITIONED:
            d = DeserializeData(msg->buf,ofs);
            dis->data_partitioned(d);
            break;
        case MQ_OPERATION_DEFINED:
            o = DeserializeOperation(msg->buf,ofs);
            operation_defined(o);
            break;


    }
}
/*=========================================================================*/


/*=========================================================================*/
extern "C" {
    IScheduler *f_create(int id)
    {
        return new MQWrapper(id);
    }
    void f_destroy(IScheduler *s)
    {
        delete s;
    }
}
Message *Queue::get(){
    int n  = msg_list.size();
    if ( n==0)
        return nullptr;
    Message *msg=msg_list.front();
    msg_list.pop_front();
    return msg;
}
Message::Message(byte * b, int s, int t)
{
    buf = b;
    len = s;
    tag = t;
}
Message::~Message(){
 delete [] buf;
}
