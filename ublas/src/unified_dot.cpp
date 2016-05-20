#include "unified_dot.hpp"
#include "unified_add.hpp"
#include "unified_copy.hpp"


unified_dot::unified_dot():GOperation()
{
    name = udot_name;
    get_dispatcher()->register_operation(this,name);
}

unified_dot::~unified_dot()
{
    //dtor
}
GTask &udot_t(GData &A,GData &B, GData &r,GTask *parent )
{
    Args *args = new Args;
    Axs axs;
    packArgs( args,A , B  , r  );
    packAxs ( axs ,In, In , Out);
    GTask *t=get_dispatcher()->submit_task(&_udot,args,axs,parent);
    return *t;
}
void udot(GData &A,GData &B, GData &r,GTask *t )
{
    int nb = A.get_part_countY();
    GData &S = *new GData (nb,1,"S");
    GPartitioner *P= new GPartitioner(nb,1);
    LOG_INFO(LOG_MLEVEL,"\n");
    S.set_partition(P);
    for(int i =0;i<nb;i++)
    {
      LOG_INFO(LOG_MLEVEL,"S(%d,0) == %s\n",i,S(i,0).get_name().c_str());
      udot_t(A(i,0),B(i,0),S(i,0),t);
    }
    for ( int j=1; j<nb; j*=2){
      for ( int i=0;i<nb;i+=2*j){
	if ( (i+j) < nb ) {
	  LOG_INFO(LOG_MLEVEL,"S(%d,0) += S(%d,0) \n",i,i+j);
	  uadd_t ( S(i+j,0) , S(i,0),t ) ;
	}
      }
    }
    LOG_INFO(LOG_MLEVEL,"copy S(0,0) -> %s(0,0) \n",r(0,0).get_name().c_str());
    ucopy_t(S(0,0),r(0,0),t);
}
void unified_dot::run(GTask *t)
{
    GData &a =* t->args->args[0];
    GData &b =* t->args->args[1];
    GData &r =* t->args->args[2];
    udot(a,b,r,t);
}
unified_dot _udot;
