#include "GPartitioner.hpp"
#include "utp_basic.hpp"
#include "dispatcher.hpp"
#include "brdcast.hpp"
#include "cmdline.hpp"
#include "GData.hpp"

extern int me;
namespace utp{
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
  BroadCast::partition_defined(this);
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
  BroadCast::partition_cascaded(this,p);
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
    utp::copy(buf,ofs,key);
    utp::copy(buf,ofs,x);
    utp::copy(buf,ofs,y);
    if ( next == nullptr){
        utp::copy(buf,ofs,invalid);
    }
    else{
        decltype(key) k= next->get_key();
        utp::copy(buf,ofs,k);
    }
}
/*-------------------------------------------------------------*/
void GPartitioner::deserialize(byte *buf,int &ofs){
    decltype(key) k;
    utp::paste(buf,ofs,&key);
    utp::paste(buf,ofs,&x);
    utp::paste(buf,ofs,&y);
    utp::paste(buf,ofs,&k);
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
  bool GPartitioner::is_owner(GData *d){
    #ifdef DT_INCLUDED     
    int level = d->get_level();
    if ( level == 0 )
      return false;
    bool dist = Dispatcher::is_distributed(level);
    if (!dist)
      return is_owner(d->get_parent());
    int row,col,depth;
    d->getCoordination(row,col,depth);
    int owner = (row % y) * cmdLine.q + col %x;
    return owner == me;
    #else

    return true;
    #endif
    
    
  }
/*-------------------------------------------------------------*/

GPartitioner * DeserializePartitioner(byte *buf,int &ofs)
{
    int k;
    utp::paste(buf,ofs,&k);
    if (k>=0 && (unsigned)k<part_list->size())
        return (*part_list)[k];
    return nullptr;
}
GPartitioner * CreatePartition(byte * buf,int &ofs)
{
    int y,x,k;
    utp::paste(buf,ofs,&k);
    utp::paste(buf,ofs,&y);
    utp::paste(buf,ofs,&x);
    GPartitioner *p = new GPartitioner(y,x);
    assert(k == p->get_key());
    return p;
}
}// namespace utp
