#include "unified_gemm.hpp"
#include <cstdlib>
#include <iostream>
unified_gemm::unified_gemm():GOperation()
{
    name = ugemm_name;
    get_dispatcher()->register_operation(this,name);
}

unified_gemm::~unified_gemm()
{
    //dtor
}
GTask &ugemm_t(GData &a,GData &b,GData &c,GTask *parent)
{
    Args *args = new Args;
    Axs axs;
    packArgs( args,a  ,b  ,c     );
    packAxs ( axs ,In ,In ,InOut );
    GTask *t=get_dispatcher()->submit_task(&_ugemm,args,axs,parent);
    return *t;
}
void ugemm(GData &a,GData &b,GData &c,GTask *parent)
{
    int m = a.get_part_countY();
    int n = b.get_part_countX();
    int o = c.get_part_countX();
    for ( int i = 0; i<m;i++)
    {
        for ( int j = 0; j<n;j++)
        {
            for ( int k = 0;k<o;k++)
            {
                ugemm_t(a(i,k),b(k,j),c(i,j),parent);
            }
        }
    }
}
void ugemm_dec(GData &a,GData &b,GData &c,GTask *parent)
{
    int m = a.get_part_countY();
    int n = b.get_part_countX();
    int o = c.get_part_countX();
    for ( int i = 0; i<m;i++)
    {
        for ( int j = 0; j<n;j++)
        {
            for ( int k = 0;k<o;k++)
            {
                ugemm_t(a(i,k),b(k,j),c(i,j),parent)
		  .Alpha(-1.0);
            }
        }
    }
}

void unified_gemm::run(GTask *t)
{
    GData &a =* t->args->args[0];
    GData &b =* t->args->args[1];
    GData &c =* t->args->args[2];
    ugemm(a,b,c,t);
}
unified_gemm _ugemm;
