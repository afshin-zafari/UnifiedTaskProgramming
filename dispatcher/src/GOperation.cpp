#include "GOperation.hpp"
#include "basic.hpp"
#include <vector>

/*-------------------------------------------------------------*/
//GOperation::GOperation(){}
/*-------------------------------------------------------------*/
//GOperation::~GOperation(){}
/*-------------------------------------------------------------*/
std::vector<GOperation*> all_operations;

GOperation::GOperation(const GOperation& other)
{
    //copy ctor
    key = all_operations.size();
    all_operations.push_back(this);

}
/*-------------------------------------------------------------*/
GOperation& GOperation::operator=(const GOperation& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
int GOperation::get_key()
{
    return key;
}
void GOperation::serialize(byte *buf, int &ofs)
{
    copy<int>(buf,ofs,key);
}
void GOperation::deserialize(byte *buf, int &ofs)
{
    paste(buf,ofs,&key);
}
