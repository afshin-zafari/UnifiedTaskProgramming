#include "GOperation.hpp"

GOperation::GOperation()
{
    //ctor
}

GOperation::~GOperation()
{
    //dtor
}

GOperation::GOperation(const GOperation& other)
{
    //copy ctor
}
//void GOperation::run(GTask *t){}

GOperation& GOperation::operator=(const GOperation& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
