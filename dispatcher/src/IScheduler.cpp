#include "IScheduler.hpp"

IScheduler::IScheduler(int id)
{
    ID = id;
    next = previous = nullptr;
}

IScheduler::~IScheduler()
{
    //dtor
}

IScheduler::IScheduler(const IScheduler& other)
{
    //copy ctor
}

IScheduler& IScheduler::operator=(const IScheduler& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
uint IScheduler::get_id()
{
    return ID;
}
void IScheduler::set_name(string n)
{
  name.assign(n);
}
string IScheduler::get_name()
{
  return name;
}
