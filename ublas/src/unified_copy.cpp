/** \file utrsm.cpp
 * Unified Triangula Solve Matrix routine from BLAS
 */
#include "unified_copy.hpp"


unified_copy::unified_copy():GOperation()
{
    name = ucopy_name;
    get_dispatcher()->register_operation(this,name);
}

unified_copy::~unified_copy()
{
    //dtor
}
GTask &ucopy_t(GData &A,GData &B, GTask *parent )
{
    Args *args = new Args;
    Axs axs;
    packArgs( args,A , B    );
    packAxs ( axs ,In, InOut);
    GTask *t=get_dispatcher()->submit_task(&_ucopy,args,axs,parent);
    return *t;
}
void ucopy(GData &A,GData &B, GTask *t )
{
    int nb = A.get_part_countY();
    for(int i =0;i<nb;i++)
    {
      ucopy_t(A(i,0),B(i,0),t);
    }
}
void unified_copy::run(GTask *t)
{
    GData &a =* t->args->args[0];
    GData &b =* t->args->args[1];
    ucopy(a,b,t);
}
unified_copy _ucopy;
