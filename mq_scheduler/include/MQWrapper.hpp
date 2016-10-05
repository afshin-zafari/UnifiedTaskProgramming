#ifndef MQWRAPPER_HPP
#define MQWRAPPER_HPP

#include "IScheduler.hpp"
#include "Dispatcher.hpp"
#include "GHandle.hpp"
#include "basic.hpp"
#include <deque>


struct Message{
    int tag,len;
    byte *buf;
    Message(byte *,int ,int);
    ~Message();
};

struct Queue {
    std::deque<Message*> msg_list;
    template <class T>
    void put(T* msg,int tag);
    Message *get();
};
/*============================================================*/
class MQWrapper: public IScheduler
{
public:
    enum{
        MQ_DATA_CREATED=100,
        MQ_PARTITION_CREATED,
        MQ_PARTITION_CASCADED,
        MQ_DATA_PARTITIONED,
        MQ_DATA_ALLOCATED,
        MQ_TASK_SUBMIT,
        MQ_TASK_READY,
        MQ_TASK_FINISHED,
        MQ_OPERATION_DEFINED
    };
    Queue *mq_send,*mq_recv;
    MQWrapper(int);
     ~MQWrapper();
    MQWrapper(const MQWrapper& other,int);
    MQWrapper& operator=(const MQWrapper& other);
    void submitTask(GTask*);
    void runTask(GTask *);
    //void finishedTaskGroup(GTask*) {}
    void finishedTask(GTask *);
    void finalize();
    int init();
    void allocate_memory(GData *);
    void data_created(GData *);
    void data_partitioned(GData *);
    void operation_defined(GOperation *);
    void partition_defined(GPartitioner*);
    void partition_cascaded(GPartitioner*,GPartitioner*);
    void run_rpc(Message *);
    void import_all();
    void export_all();
protected:
private:
};
/*============================================================*/




typedef IScheduler * (*fp_create)(int);
typedef void (*fp_destroy)(IScheduler *);

#endif // MQWRAPPER_HPP
