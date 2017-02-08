#ifndef SCH_SUPERGLUE_HPP
#define SCH_SUPERGLUE_HPP
#include "operation.hpp"
#include "dispatcher.hpp"
#include "sg/superglue.hpp"
#include "sch_ductteip.hpp"

namespace utp{
    template <typename T> class OperationBase;
    template <typename T,typename P> class Task;
    class SG;
    extern SG _sg;


    /*============================================================*/
    struct SGWOptions : public DefaultOptions<SGWOptions>
    {
        typedef Disable Logging;
        typedef Enable TaskName;
    };
    SuperGlue<SGWOptions> *SG_Engine;

    /*============================================================*/
    typedef Handle<SGWOptions> sg_data_t;
    /*============================================================*/

    template <typename T,typename P>
    class SGTask:public sg::Task<SGWOptions>
    {
    public:
      typedef  utp::Task<T,P> GTask;
      GTask * gtask;
      SGTask(GTask *gt):gtask(gt)
      {
	Args *a=gt->args;
	P *p = gtask->get_parent();
	int cnt = p->child_count;
	#if UTP_DEBUG !=0
	cout << "++++ " << cnt << ", " << p->id << endl;
	#endif
	for(uint i=0; i< a->args.size(); i++)
	  {
	    sg_data_t *sgd = (sg_data_t *)a->args[i]->get_guest();
	    if ( sgd == NULL ){
	      sgd =new sg_data_t;
	      a->args[i]->set_guest((void*)sgd);
	    }
	    sg_data_t *h = sgd;//glb_to_sg[gh];
	    if ( gt->axs->axs[i]==In)
	      register_access(ReadWriteAdd::read,*h);
	    else
	      register_access(ReadWriteAdd::write,*h);
	  }

      }
      void run(){
#if SHORTCUT == 0 
	Dispatcher::ready(_sg,gtask);
#else
	gtask->o->run(gtask);
#endif
	#if UTP_DEBUG != 0
	cout << "---- \n" ;
	#endif
	P *p = gtask->get_parent();
        if ( p != nullptr){
	  if ( p->child_count !=0){
	    while (p->is_generating)
	      Atomic::yield();
	    const int new_count(Atomic::decrease_nv(&p->child_count));	  
	    if( new_count ==0){
	       int new_cn = Atomic::decrease_nv(&p->child_count);
	       #if UTP_DEBUG != 0
	       cout << "==== sg_Parent finished\t" <<  p->o->name << "_" << p->id << "cnt: "<<new_cn <<  endl;
	       #endif
	       DT::finished(p);
	     }
	  }
	}
      }
      ~SGTask(){
	/*
	#if SHORTCUT == 1
	P *p = gtask->get_parent();
        if ( p != nullptr){
	  unsigned int new_count=1;
	  new_count = p->child_count;
	  if( new_count ==0){
	    Atomic::decrease(&p->child_count);
	    cout << "==== sg_Parent finished\t" <<  p->o->name << "_" << p->id << endl;
	    DT::finished(p);
	  }	
        }
	#endif
	*/
      }
      string get_name(){
	return gtask->o->name;
      }
    };
/*============================================================*/
class SG{
public:
    static string name ;
    static int level;
    /*-----------------------------------------------------------------------------------*/
    SG(){}
    /*-----------------------------------------------------------------------------------*/
    static void data_created(GData *d){
        sg_data_t *sgd= (sg_data_t*)d->get_guest();
        if ( sgd == NULL ) {
            sgd = new sg_data_t;
            d->set_guest((void *)sgd);
        }
        PRINTF("Data %s , created at level:%d\n",d->get_name().c_str(),d->get_level() );
        if ( d->get_level() ==0 && level == 0){
            int size = d->get_cols() * d->get_rows() * sizeof(double);
            void *m =(void *) new byte[size];
	    PRINTF("%s,%d: %s\n",__FILE__,__LINE__,__FUNCTION__);
            PRINTF("set_memory  %d x %d x8 = %d\n",d->get_cols() , d->get_rows(),size);
            d->set_memory(m,d->get_rows());
            memset ( m,0,size);
        }
        else if ( d->get_level() != 0){
            byte *m =(byte *) d->get_parent()->get_memory();
            int offset = d->get_child_index() *d->get_rows() * d->get_cols() * sizeof(double);
	    if ( m == nullptr)
	      offset =0;
	    PRINTF("%s,%d: %s\n",__FILE__,__LINE__,__FUNCTION__);
	    d->set_memory(m+offset,  d->get_parent()->get_ld());
            PRINTF("Data %s, parent memory %p, child memory %p\n",d->get_name().c_str(),m,m+offset);
        }
    }
    /*-----------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
#     if DEBUG != 0
        cout << "----\t  SG.ready\t" << t->o->name << "_" << t->id << endl;
#     endif
        Dispatcher::ready(_sg,t);
    }
    /*-----------------------------------------------------------------------------------*/
    template<typename T,typename P>
    static inline int submit(Task<T,P>*t){
#     if DEBUG != 0
        cout << "----\t  SG.submit\t" << t->o->name << "_" << t->id << endl;
#     endif
        SGTask<T,P> *st = new SGTask<T,P> ( t);
        SG_Engine->submit(st);
        return 1;
    }
    /*-----------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
#     if DEBUG != 0
        std::cout << "----\t  SG.finished\t" << t->o->name << "_" << t->id << endl;
#     endif
        Dispatcher::finished(_sg,t);
    }
    /*-----------------------------------------------------------------------------------*/
    static void data_partitioned(GData *d ){
        int by = d->get_part_countY();
        int bx = d->get_part_countX();
        int m = d->get_rows();
        int n = d->get_cols();
        int child_m = m/by;
        int child_n = n/bx;
        int mn = child_m*child_n;
        if ( mn == 1)
            return;
        if ( d->get_level() == 0){
            PRINTF("Skip this partitioning %dx%d %dx%d at level:%d.\n",by,bx,m,n,d->get_level());
            return;
        }
        double *content = (double *)d->get_memory();
        PRINTF("parent gdata memory:%p \n",d->get_memory());
        for ( int i =0 ; i < by; i++){
            for ( int j =0 ; j < bx; j++){
              GData &d_ch = (*d)(i,j);
              PRINTF("gd_ch:%s\n",d_ch.get_name().c_str());
	      PRINTF("%s,%d: %s\n",__FILE__,__LINE__,__FUNCTION__);
	      if ( content == nullptr)
		d_ch.set_memory((void*)(nullptr), -1);
	      else
		d_ch.set_memory((void*)(content+(j*by+i)*mn),d->get_ld());
              PRINTF("child (%d,%d) gdata memory:%p lead_dim:%d\n",i,j,d_ch.get_memory(),d_ch.get_ld());
            }
        }
    }
    /*-----------------------------------------------------------------------------------*/
    static void partition_defined(GPartitioner *p){}
    static void partition_cascaded(GPartitioner *p1,GPartitioner *p2){}
    /*-----------------------------------------------------------------------------------*/


};
/*============================================================*/
}// namespace utp

#endif // SCH_SUPERGLUE_HPP
