#ifndef GPARTITIONER_HPP
#define GPARTITIONER_HPP
#include <vector>
namespace utp{
  typedef unsigned char byte;
  class GData;
class GPartitioner
{
    public:
        GPartitioner();
        virtual ~GPartitioner();
        GPartitioner (int , int ) ;
        GPartitioner(const GPartitioner& other);
        GPartitioner& operator=(const GPartitioner& other);
        int y,x;
        void set_next(GPartitioner *p);
        GPartitioner *get_next();
        bool is_for_comm();
        bool is_for_allocation();
        bool is_for_ownership();
        void set_for_comm(bool);
        void set_for_allocation(bool);
        void set_for_ownership(bool);
        void serialize(byte *buf,int &offset);
        void deserialize(byte *buf, int &offset);
        int get_key();
        virtual bool is_owner(GData *);
    protected:
        GPartitioner  *next;
        int key;
    private:
        bool comm,ownership,alloc;
};
extern std::vector<GPartitioner*> *part_list;
GPartitioner * DeserializePartitioner(byte *,int &);
GPartitioner *CreatePartition(byte * buf, int &ofs);
}//namespace utp
#endif // GPARTITIONER_HPP
