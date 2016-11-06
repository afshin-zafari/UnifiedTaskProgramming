#include "ublas.hpp"
namespace utp{
    template <typename S>
    void ready(S &s,unsigned int id, unsigned int type)
    {
    }
    template <>
    void ready<SG>(SG &s,unsigned int id, unsigned int type)
    {
    }
}//namespace utp

