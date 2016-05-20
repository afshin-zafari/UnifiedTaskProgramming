#include "unified_add.hpp"


unified_add::unified_add():GOperation()
{
    name = uadd_name;
    get_dispatcher()->register_operation(this,name);
}

unified_add::~unified_add()
{
    //dtor
}
GTask &uadd_t(GData &A,GData &B, GTask *parent )
{
    Args *args = new Args;
    Axs axs;
    packArgs( args,A , B   );
    packAxs ( axs ,In, Out);
    GTask *t=get_dispatcher()->submit_task(&_uadd,args,axs,parent);
    return *t;
}
void uadd(GData &A,GData &B, GTask *t )
{
    int nb = A.get_part_countY();
    for(int i =0;i<nb;i++)
    {
      uadd_t(A(i,0),B(i,0),t);
    }
}
void unified_add::run(GTask *t)
{
    GData &a =* t->args->args[0];
    GData &b =* t->args->args[1];
    uadd(a,b,t);
}
unified_add _uadd;
