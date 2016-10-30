#include "sch_superglue.hpp"

namespace utp{
        SG _sg;


    /****************************************************************************/
    /*
    template < typename T>
    SGTask::SGTask(GTask *t):gtask(t)
    {
        Args *a=t->args;
        printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);

        for(uint i=0; i< a->args.size(); i++)
        {
          //GHandleKey gh = a->args[i]->get_handle()->get_key();
        sg_data_t *sgd = (sg_data_t *)a->args[i]->get_guest();
        if ( sgd == NULL ){
                sgd =new sg_data_t;
            a->args[i]->set_guest((void*)sgd);
        }
        //        if (glb_to_sg[gh]==NULL)            glb_to_sg[gh]=new sg_data_t;
            sg_data_t *h = sgd;//glb_to_sg[gh];
        printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);
            if ( a->args[i]->axs==In)
                register_access(ReadWriteAdd::read,*h);
            else
                register_access(ReadWriteAdd::write,*h);
        printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);
        }
        printf("%s,%s,%d\n",__FILE__,__FUNCTION__,__LINE__);
    }
    */
    /*=========================================================================*/
    /*=========================================================================*/
}//namespace utp
