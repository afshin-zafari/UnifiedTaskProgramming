#include "GPartitioner.hpp"

GPartitioner::GPartitioner()
{
    //ctor
  next = nullptr;
}
GPartitioner::GPartitioner(int i , int j):y(i),x(j)
{
  next = nullptr;
}

GPartitioner::~GPartitioner()
{
    //dtor
}

GPartitioner::GPartitioner(const GPartitioner& other)
{
    //copy ctor
}

GPartitioner& GPartitioner::operator=(const GPartitioner& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
void GPartitioner::set_next(GPartitioner *p)
{
  next = p;
}
GPartitioner *GPartitioner::get_next()
{
  return next;
}
