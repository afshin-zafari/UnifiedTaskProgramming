#include "GOperation.hpp"
#include "basic.hpp"
#include <vector>
std::vector<GOperation*> *all_operations;

/*-------------------------------------------------------------*/
GOperation::GOperation(){
    if ( all_operations == nullptr )
        all_operations = new std::vector <GOperation*>;
    key = all_operations->size();
    all_operations->push_back(this);
}
/*-------------------------------------------------------------*/
//GOperation::~GOperation(){}
/*-------------------------------------------------------------*/

GOperation::GOperation(const GOperation& other)
{
    //copy ctor

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
GOperation *DeserializeOperation(byte *buf, int &ofs)
{
    unsigned int k;
    paste(buf,ofs,&k);
    if (k>=0 and k<all_operations->size()){
        return (*all_operations)[k];
    }
    return nullptr;
}
