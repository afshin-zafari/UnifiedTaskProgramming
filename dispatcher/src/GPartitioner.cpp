#include "GPartitioner.hpp"
#include "basic.hpp"
#include "Dispatcher.hpp"
std::vector<GPartitioner*> *part_list;
/*-------------------------------------------------------------*/
GPartitioner::GPartitioner()
{
    //ctor
  next = nullptr;
  if ( part_list == nullptr)
    part_list = new std::vector<GPartitioner*>;
  key = part_list->size();
  part_list->push_back(this);
}
/*-------------------------------------------------------------*/
GPartitioner::GPartitioner(int i , int j):y(i),x(j)
{
  next = nullptr;
  if ( part_list == nullptr)
    part_list = new std::vector<GPartitioner*>;
  key = part_list->size();
  part_list->push_back(this);
  Dispatcher *dis= get_dispatcher();
  if ( dis)
    dis->partition_defined(this);
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
  Dispatcher *dis= get_dispatcher();
  if(dis)
    dis->partition_cascaded(this,p);
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
void GPartitioner::serialize(byte *buf,int &ofs)
{
    decltype(key) invalid=-1;
    copy(buf,ofs,key);
    copy(buf,ofs,x);
    copy(buf,ofs,y);
    if ( next == nullptr){
        copy(buf,ofs,invalid);
    }
    else{
        decltype(key) k= next->get_key();
        copy(buf,ofs,k);
    }
}
/*-------------------------------------------------------------*/
void GPartitioner::deserialize(byte *buf,int &ofs){
    decltype(key) k;
    paste(buf,ofs,&key);
    paste(buf,ofs,&x);
    paste(buf,ofs,&y);
    paste(buf,ofs,&k);
    if (k)
        next = (*part_list)[k];
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
    int k;
    paste(buf,ofs,&k);
    if (k>=0 && (unsigned)k<part_list->size())
        return (*part_list)[k];
    return nullptr;
}
GPartitioner * CreatePartition(byte * buf,int &ofs)
{
    int y,x,k;
    paste(buf,ofs,&k);
    paste(buf,ofs,&y);
    paste(buf,ofs,&x);
    GPartitioner *p = new GPartitioner(y,x);
    assert(k == p->get_key());
    return p;
}
