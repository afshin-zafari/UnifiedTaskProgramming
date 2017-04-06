#include "basic.hpp"
#include "GOperation.hpp"
#include "GTask.hpp"
#include "Dispatcher.hpp"
/*==================================================================*/
std::vector<GTask*> *all_tasks;
GTask::GTask()
{
    task_count = 0;
    handle = new GHandle;
    if(all_tasks == nullptr)
        all_tasks = new std::vector<GTask *>;
    key=all_tasks->size();
    all_tasks->push_back(this);
}
/*==================================================================*/
GTask::GTask(string fn,Args * a, FUT key):
    fname(fn),args(a)
{
    task_count = 0;
    handle = new GHandle;
    alpha = beta = 1.0;
    trans_a = trans_b = trans_c = false;
    upper_tr = lower_tr= false;
    unit_diagonal=false;
    left_side = true;
    right_side=!left_side;
    if(all_tasks == nullptr)
        all_tasks = new std::vector<GTask *>;
    key=all_tasks->size();
    all_tasks->push_back(this);

}
/*==================================================================*/
GTask::~GTask()
{
    //dtor
}
/*==================================================================*/
GTask::GTask(const GTask& other)
{
    //copy ctor
}
/*==================================================================*/
GTask& GTask::operator=(const GTask& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*==================================================================*/
GTask *GTask::get_parent()
{
    return parent;
}
/*==================================================================*/
void GTask::set_parent(GTask *p)
{
    parent = p;
    if ( !p )
      return;
      #ifndef LOCAL_DEV
    Atomic::increase(&p->task_count);
    #endif // LOCAL_DEV
}
/*==================================================================*/
IScheduler *GTask::get_owner()
{
    return owner;
}
/*==================================================================*/
void GTask::set_owner(IScheduler *o)
{
    owner =o;
}
/*==================================================================*/
GTask &GTask::Beta(double v)
{
    beta = v;
    return *this;
}
/*==================================================================*/
double GTask::get_beta()
{
    return beta;
}
/*==================================================================*/
GTask &GTask::Alpha(double v)
{
    alpha = v;
    return *this;
}
/*==================================================================*/
double GTask::get_alpha()
{
    return alpha;
}
/*==================================================================*/
GTask &GTask::TransA(bool v)
{
    trans_a= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_trans_a()
{
    return trans_a;
}
/*==================================================================*/
GTask &GTask::TransB(bool v)
{
    trans_b= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_trans_b()
{
    return trans_b;
}
/*==================================================================*/
GTask &GTask::TransC(bool v)
{
    trans_c= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_trans_c()
{
    return trans_c;
}
/*==================================================================*/
GTask &GTask::LeftSide(bool v)
{
    left_side= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_left_side()
{
    return left_side;
}
/*==================================================================*/
GTask &GTask::RightSide(bool v)
{
    right_side= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_right_side()
{
    return right_side;
}
/*==================================================================*/
GTask &GTask::UpperTriangle(bool v)
{
    upper_tr= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_upper_triangle()
{
    return upper_tr;
}
/*==================================================================*/
GTask &GTask::LowerTriangle(bool v)
{
    lower_tr= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_lower_triangle()
{
    return lower_tr;
}
/*==================================================================*/
GTask &GTask::UnitDiagonal(bool v)
{
    unit_diagonal= v;
    return *this;
}
/*==================================================================*/
bool GTask::get_unit_diagonal()
{
    return unit_diagonal;
}
/*==================================================================*/
GHandle * GTask::get_handle()
{
    return handle;
}
/*==================================================================*/
string GTask::get_name()
{
  return fname;
}
/*==================================================================*/
void* GTask::get_guest(){return guest;}
void GTask::set_guest(void *p){guest = p;}
/*==================================================================*/
void GTask::set_operation(GOperation*p){operation = p;}
GOperation *GTask::get_operation(){return operation;}
/*==================================================================*/
int GTask::get_key()
{
    return key;
}

void GTask::serialize(byte *buf, int &ofs)
{
    int invalid=-1;
    copy(buf,ofs,key);
    copy(buf,ofs,handle->get_key());
    if(parent)
        copy(buf,ofs,parent->get_key());
    else
        copy(buf,ofs,invalid);
    if(operation)
        operation->serialize(buf,ofs);
    else
        copy(buf,ofs,invalid);
    if (owner)
        copy(buf,ofs,owner->get_id());
    else
        copy(buf,ofs,invalid);
    int n =args->args.size();
    copy(buf,ofs,n);
    for(int i=0;i<n;i++)
    {
        copy(buf,ofs,args->args[i]->get_handle()->get_key());
        copy(buf,ofs,args->axs.axs[i]);
    }
}
void GTask::deserialize(byte *buf, int &ofs)
{
    int k,p,o,s,n,d;
    GHandleKey h;
    int invalid=-1;
    paste(buf,ofs,&k);
    if ( k != invalid)
        key = k;
    paste(buf,ofs,&h);
    // if ( h != invalid) //TODO: Then What??

    paste(buf,ofs,&p);
    if(p!=invalid){
        ofs -= sizeof(p);
        parent=DeserializeTask(buf,ofs);
    }

    paste(buf,ofs,&o);
    if(o!=invalid){
        ofs -= sizeof(o);
        operation = DeserializeOperation(buf,ofs);
    }
    paste(buf,ofs,&s);
    if (s!=invalid){
        owner = get_dispatcher()->get_scheduler(s);
    }
    paste(buf,ofs,&n);
    int x;
    args = new Args;
    for(int i=0;i<n;i++)
    {
        paste(buf,ofs,&d);
        paste(buf,ofs,&x);
        GData *data=(*all_data)[d];
        args->axs.addAxs(x);
        args->args.push_back(data);
    }
}
GTask *DeserializeTask(byte *buf,int &ofs)
{
    int k;
    paste(buf,ofs,&k);
    if( k>= 0 and (unsigned)k<all_tasks->size()){
        return (*all_tasks)[k];
    }
    return nullptr;
}
GTask *CreateTask(byte *buf, int &ofs)
{
    GTask *t = new GTask();
    t->deserialize(buf,ofs);
    return t;
}
Args *GTask::get_args()
{
    return args;
}
Axs *GTask::get_axs()
{
    return &(args->axs);
}
