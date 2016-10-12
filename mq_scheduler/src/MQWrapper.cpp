#include "MQWrapper.hpp"
#include "GTask.hpp"
#include "GData.hpp"
#include "GPartitioner.hpp"
#include <stdlib.h>

#define LOG(x) printf("%s, %s\n",__PRETTY_FUNCTION__,x)
Queue::Queue(mq::MQ *m):mq_sch(m){}
template <>
void Queue::put(GData *d,int tag){

    byte *buf = new byte[128];
    int ofs=0;
    copy(buf,ofs,tag);
    d->serialize(buf,ofs);
    Message *msg = new Message ( buf, ofs, tag);
    msg_list.push_back(msg);
    mq_sch->send_buffer((const char *)buf,ofs);
}
template <>
void Queue::put(GTask *t,int tag){
    byte *buf=new byte[256];
    int ofs=0;
    copy(buf,ofs,tag);
    t->serialize(buf,ofs);
    Message *msg = new Message ( buf, ofs, tag);
    msg_list.push_back(msg);
    mq_sch->send_buffer((const char *)buf,ofs);
}
template <>
void Queue::put(GPartitioner *p,int tag){
    byte *buf=new byte[256];
    int ofs=0;
    copy(buf,ofs,tag);
    p->serialize(buf,ofs);
    Message *msg = new Message ( buf, ofs, tag);
    msg_list.push_back(msg);
    mq_sch->send_buffer((const char *)buf,ofs);
}

const int In   = 1;
const int Out  = 2;
const int InOut= 3;

/*=========================================================================*/
MQWrapper::MQWrapper(int id):IScheduler(id)
{
  LOG("");
  mq_sch = new mq::MQ();
  LOG("");
  mq_send = new Queue(mq_sch);
  mq_recv = new Queue(mq_sch);
  t = new boost::thread ([]{mq::ioService->run();});
}

/*=========================================================================*/
int MQWrapper::init()
{
    return 0;
}
/*=========================================================================*/
MQWrapper::~MQWrapper()
{
  delete t;  
  delete mq_send;
  delete mq_recv;
  delete mq_sch;
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
    LOG("");

}
/*=========================================================================*/
void MQWrapper::runTask(GTask *t)
{
    mq_send->put(t,MQ_TASK_READY);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::finishedTask(GTask *t)
{
    mq_send->put(t,MQ_TASK_FINISHED);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::finalize()
{
  if(t!= nullptr)
    t->join();
}
/*=========================================================================*/
void MQWrapper::allocate_memory(GData*d)
{
    mq_send->put(d,MQ_DATA_ALLOCATED);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::data_created(GData *d)
{
    mq_send->put(d,MQ_DATA_CREATED);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::data_partitioned(GData *d)
{
    mq_send->put(d,MQ_DATA_PARTITIONED);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::partition_defined(GPartitioner*p)
{
    mq_send->put(p,MQ_PARTITION_CREATED);
    LOG("");
}
/*=========================================================================*/
void MQWrapper::run_rpc(Message *msg ){
    Dispatcher * dis = get_dispatcher();
    GTask *t;
    GData *d;
    GPartitioner *p;
    int ofs=0;
    switch(msg->tag){
        case MQ_TASK_FINISHED:
            LOG("TASK Finished");
            t = DeserializeTask(msg->buf,ofs);
            dis->finished_task(t);
            break;
        case MQ_TASK_READY:
            LOG("Task Ready");
            t = DeserializeTask(msg->buf,ofs);
            dis->run_task(t);
            break;
        case MQ_TASK_SUBMIT:
            LOG("Task submit");
            t = CreateTask(msg->buf,ofs);
            dis->submit_task(t->get_operation(),t->get_args(),*t->get_axs(),t->get_parent());
            break;
        case MQ_DATA_ALLOCATED:
            LOG("data alloc");
            d = DeserializeData(msg->buf,ofs);
            dis->allocate_memory(d);
            break;
        case MQ_DATA_CREATED:
            LOG("Data Created");
            d = CreateData (msg->buf,ofs);
            break;
        case MQ_DATA_PARTITIONED:
            LOG("Data Partitioned");
            d = DeserializeData(msg->buf,ofs);
            ofs += 6 * sizeof(int);
            p = DeserializePartitioner(msg->buf,ofs);
            if ( d->get_partition() != p)
                d->set_partition(p);
            break;
        case MQ_OPERATION_DEFINED:
            //o = DeserializeOperation(msg->buf,ofs);
            //operation_defined(o);
            break;
        case MQ_PARTITION_CREATED:
            LOG("Partition created");
            p = CreatePartition(msg->buf,ofs);

            break;
        case MQ_PARTITION_CASCADED:
            LOG("Partition Cascaded");
            p=DeserializePartitioner(msg->buf,ofs);
            ofs += 2*sizeof(int);
            GPartitioner *p2=DeserializePartitioner(msg->buf,ofs);
            p->set_next(p2);

            break;

    }
}
/*=========================================================================*/
void MQWrapper::import_all()
{
    Message *m;

    FILE *f;
    f=fopen("msg_sent.dat","r");
    if ( f == NULL)
    return;
    while(!feof(f)){


        int n,tag;
//        decltype<Message::tag> tag ;
        fread(&tag,sizeof(tag),1,f);
        fread(&n,sizeof(n),1,f);
        byte * buf = new byte[n];
        fread(buf,1,n,f);
        m = new Message(buf,n,tag);
        run_rpc(m);
        mq_recv->msg_list.push_back(m);
    };
    fclose(f);
}
/*=========================================================================*/
void MQWrapper::export_all()
{
    Message *m;

    FILE *f;
    f=fopen("msg_sent.dat","w");
    do{
        m=mq_send->get();
        if (!m)break;//run_rpc(m);
        int n = m->len;
        fwrite(&m->tag,sizeof(m->tag),1,f);
        fwrite(&n,sizeof(n),1,f);
        fwrite(m->buf,1,n,f);
    }while (m);
    fclose(f);
}
/*=========================================================================*/
void MQWrapper::partition_cascaded(GPartitioner*p1,GPartitioner*p2)
{
    mq_send->put(p1,MQ_PARTITION_CASCADED);
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
