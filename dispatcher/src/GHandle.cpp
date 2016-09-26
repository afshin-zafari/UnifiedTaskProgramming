#include "GHandle.hpp"
#include "basic.hpp"
GHandleKey GHandle::lastKey=0;
/*-------------------------------------------------------------*/
GHandle::GHandle()
{
    key = lastKey++;
}
/*-------------------------------------------------------------*/
GHandle::~GHandle()
{
    //dtor
}
/*-------------------------------------------------------------*/
GHandle::GHandle(const GHandle& other)
{
    //copy ctor
}
/*-------------------------------------------------------------*/
GHandle& GHandle::operator=(const GHandle& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
/*-------------------------------------------------------------*/
GHandleKey GHandle::get_key()
{
    return key;
}
void GHandle::serialize(byte *buf, int &ofs)
{
    copy(buf,ofs,key);
}
void GHandle::deserialize(byte *buf, int &ofs)
{
    paste(buf,ofs,&key);
}
