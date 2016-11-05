#include "dispatcher.hpp"
#include "sch_superglue.hpp"

namespace utp{
    template <typename E>
    void EdgeDispatch<E>::data_created(GData *d){
        E::First::data_created(d);
        E::Second::data_created(d);
    }
}
