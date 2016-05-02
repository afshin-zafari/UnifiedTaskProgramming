#include "GPartitioner.hpp"

/*-------------------------------------------------------------*/
GPartitioner::GPartitioner()
{
    //ctor
  next = nullptr;
}
/*-------------------------------------------------------------*/
GPartitioner::GPartitioner(int i , int j):y(i),x(j)
{
  next = nullptr;
}
/*-------------------------------------------------------------*/
GPartitioner::~GPartitioner()
{
    //dtor
}
/*-------------------------------------------------------------*/
GPartitioner::GPartitioner(const GPartitioner& other)
{
    //copy ctor
}
/*-------------------------------------------------------------*/
GPartitioner& GPartitioner::operator=(const GPartitioner& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*-------------------------------------------------------------*/
void GPartitioner::set_next(GPartitioner *p)
{
  next = p;
}
/*-------------------------------------------------------------*/
GPartitioner *GPartitioner::get_next()
{
  return next;
}
/*-------------------------------------------------------------*/
bool GPartitioner::is_for_comm(){return comm;}
bool GPartitioner::is_for_allocation(){return alloc;}
bool GPartitioner::is_for_ownership(){return ownership;}
void GPartitioner::set_for_comm(bool f){comm = f;}
void GPartitioner::set_for_allocation(bool f){alloc = f;}
void GPartitioner::set_for_ownership(bool f){ownership = f;}
/*-------------------------------------------------------------*/
