#ifndef SGWRAPPER_HPP
#define SGWRAPPER_HPP

#include "IScheduler.hpp"
#include "sg/superglue.hpp"
#include "Dispatcher.hpp"
#include "GHandle.hpp"
#include "basic.hpp"

#include <map>
using namespace sg;

/*============================================================*/
class SGWrapper: public IScheduler
{
public:
    SGWrapper(int);
     ~SGWrapper();
    SGWrapper(const SGWrapper& other,int);
    SGWrapper& operator=(const SGWrapper& other);
    void submitTask(GTask*);
    void runTask(GTask *);
    //void finishedTaskGroup(GTask*) {}
    void finishedTask(GTask *);
    void finalize();
    int init();
    void allocate_memory(GData *);
    void data_created(GData *);
    void data_partitioned(GData *);
protected:
private:
};
/*============================================================*/

struct SGWOptions : public DefaultOptions<SGWOptions>
{
    //typedef MyHandle<Options> HandleType;
    typedef Enable Logging;
    typedef Enable TaskName;
};

/*============================================================*/
typedef Handle<SGWOptions> sg_data_t;
/*============================================================*/
class SGGenTask:public Task<SGWOptions>
{
private:
    SGWrapper *scheduler;
    GTask *gt;
    int k;
public:
    SGGenTask(SGWrapper *w,GTask *t);
    void run();
    string get_name();
    ~SGGenTask();
};
/*============================================================*/


//extern map<GHandleKey,sg_data_t*> glb_to_sg;
typedef IScheduler * (*fp_create)(int);
typedef void (*fp_destroy)(IScheduler *);
extern SuperGlue<SGWOptions> *SG;
#endif // SGWRAPPER_HPP
