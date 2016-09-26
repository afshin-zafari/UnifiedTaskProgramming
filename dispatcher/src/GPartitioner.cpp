#include "GPartitioner.hpp"
#include "basic.hpp"
std::vector<GPartitioner*> part_list;
/*-------------------------------------------------------------*/
GPartitioner::GPartitioner()
{
    //ctor
  next = nullptr;
  key = part_list.size();
  part_list.push_back(this);
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
int GPartitioner::get_key(){
    return key;
}
/*-------------------------------------------------------------*/
void GPartitioner::serialize(byte *buf,int &ofs){
    copy(buf,ofs,x);
    copy(buf,ofs,y);
    if ( next == nullptr){
        copy(buf,ofs,0);
    }
    else{
        int k = next->get_key();
        copy(buf,ofs,k);
    }
}
/*-------------------------------------------------------------*/
void GPartitioner::deserialize(byte *buf,int &ofs){
    int k;
    paste(buf,ofs,&x);
    paste(buf,ofs,&y);
    paste(buf,ofs,&k);
    if (k)
        next = part_list[k];
    else
        next = nullptr;
}
bool GPartitioner::is_for_comm(){return comm;}
bool GPartitioner::is_for_allocation(){return alloc;}
bool GPartitioner::is_for_ownership(){return ownership;}
void GPartitioner::set_for_comm(bool f){comm = f;}
void GPartitioner::set_for_allocation(bool f){alloc = f;}
void GPartitioner::set_for_ownership(bool f){ownership = f;}
/*-------------------------------------------------------------*/

GPartitioner * DeserializePartitioner(byte *buf,int &ofs)
{
    int k,not_used;
    paste(buf,ofs,&not_used);
    paste(buf,ofs,&not_used);
    paste(buf,ofs,&k);
    if (k>0 && (unsigned)k<part_list.size())
        return part_list[k];
    return nullptr;
}
