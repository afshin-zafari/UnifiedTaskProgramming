#include "GTask.hpp"
/*==================================================================*/
GTask::GTask()
{
  task_count = 0;
    handle = new GHandle;
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
    Atomic::increase(&p->task_count);
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
