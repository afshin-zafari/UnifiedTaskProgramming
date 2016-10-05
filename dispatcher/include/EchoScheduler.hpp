#ifndef EchoScheduler_HPP
#define EchoScheduler_HPP

#include "IScheduler.hpp"
//#include "sg/superglue.hpp"
#include "Dispatcher.hpp"
#include "GHandle.hpp"
#include "basic.hpp"
#include <deque>


/*============================================================*/
class EchoScheduler: public IScheduler
{
public:
    EchoScheduler(int);
     ~EchoScheduler();
    EchoScheduler(const EchoScheduler& other,int);
    EchoScheduler& operator=(const EchoScheduler& other);
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
    void run_rpc();
protected:
private:
};
/*============================================================*/




typedef IScheduler * (*fp_create)(int);
typedef void (*fp_destroy)(IScheduler *);

#endif // EchoScheduler_HPP
