/** \file utrsm.cpp
 * Unified Triangula Solve Matrix routine from BLAS
 */
#include "unified_trsm.hpp"
#include "unified_gemm.hpp"


unified_trsm::unified_trsm():GOperation()
{
    name = utrsm_name;
    get_dispatcher()->register_operation(this,name);
}

unified_trsm::~unified_trsm()
{
    //dtor
}
GTask &utrsm_t(GData &A,GData &B, GTask *parent )
{
    Args *args = new Args;
    Axs axs;
    packArgs( args,A ,B    );
    packAxs ( axs ,In,InOut);
    GTask *t=get_dispatcher()->submit_task(utrsm_name,args,axs,parent);
    return *t;
}
void utrsm(GData &A,GData &B, GTask *t )
{
    int nb = A.get_part_countY();

    for(int i =0;i<nb;i++)
    {
        for(int j = 0;j<nb;j++)
        {
            for(int k= 0;k<i;k++)
            {
                ugemm_t(B(j,k),A(i,k),B(j,i))
		  .Alpha(-1)
		  .Beta(1.0);
            }
            utrsm_t(A(i,i),B(j,i));
        }
    }
}
void unified_trsm::run(GTask *t)
{
    GData &a =* t->args->args[0];
    GData &b =* t->args->args[1];
    utrsm(a,b,t);
}
unified_trsm *_utrsm;
