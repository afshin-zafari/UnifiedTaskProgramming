#ifndef SCH_SUPERGLUE_HPP
#define SCH_SUPERGLUE_HPP
#include "operation.hpp"
#include "dispatcher.hpp"
#include "sg/superglue.hpp"

namespace utp{
    template <typename T> class OperationBase;
    template <typename T,typename P> class Task;
    class SG;
    extern SG _sg;


    /*============================================================*/
    struct SGWOptions : public DefaultOptions<SGWOptions>
    {
        typedef Enable Logging;
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
                Dispatcher::ready(_sg,gtask);
            }
            string get_name(){
                return gtask->o->name;
            }
    };
/*============================================================*/
class SG{
public:
    string name ;
    /*-----------------------------------------------------------------------------------*/
    SG():name("SG"){}
    /*-----------------------------------------------------------------------------------*/
    static void data_created(GData *d){
        sg_data_t *sgd= (sg_data_t*)d->get_guest();
        if ( sgd == NULL ) {
            sgd = new sg_data_t;
            d->set_guest((void *)sgd);
        }
        printf("Data %s , created at level:%d\n",d->get_name().c_str(),d->get_level() );
        if ( d->get_level() ==0){
            int size = d->get_cols() * d->get_rows() * sizeof(double);
            void *m =(void *) new byte[size];
            printf("memory %d x %d x8 = %d\n",d->get_cols() , d->get_rows(),size);
            d->set_memory(m);
            memset ( m,0,size);
        }
        else{
            byte *m =(byte *) d->get_parent()->get_memory();
            int offset = d->get_child_index() *d->get_rows() * d->get_cols() * sizeof(double);
            d->set_memory(m+offset);
            printf("Data %s, parent memory %p, child memory %p\n",d->get_name().c_str(),m,m+offset);
        }
    }
    /*-----------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void ready(Task<T,P> *t){
        cout << "----\t  SG.ready\t" << t->o->name << "_" << t->id << endl;
        Dispatcher::ready(_sg,t);
    }
    /*-----------------------------------------------------------------------------------*/
    template<typename T,typename P>
    static inline int submit(Task<T,P>*t){
        cout << "----\t  SG.submit\t" << t->o->name << "_" << t->id << endl;
        SGTask<T,P> *st = new SGTask<T,P> ( t);
        cout << st  << endl;
        SG_Engine->submit(st);
        return 1;
    }
    /*-----------------------------------------------------------------------------------*/
    template <typename T,typename P>
    static inline void finished(Task<T,P> *t){
        std::cout << "----\t  SG.finished\t" << t->o->name << "_" << t->id << endl;
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
            printf("Skip this partitioning %dx%d %dx%d at level:%d.\n",by,bx,m,n,d->get_level());
            return;
        }
        double *content = (double *)d->get_memory();
        printf("parent gdata memory:%p \n",d->get_memory());
        for ( int i =0 ; i < by; i++){
            for ( int j =0 ; j < bx; j++){
              GData &d_ch = (*d)(i,j);
              printf("gd_ch:%s\n",d_ch.get_name().c_str());
              d_ch.set_memory((void*)(content+(j*by+i)*mn));
              printf("child (%d,%d) gdata memory:%p \n",i,j,d_ch.get_memory());
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
