#ifndef GDATA_HPP
#define GDATA_HPP

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>
#include "utp_basic.hpp"
#include "GHandle.hpp"
#include "GPartitioner.hpp"
#include "mem_manager.hpp"

namespace utp {
class GData
{
    public:
        int axs;
        GData();
        GData ( int , int ,string ="");
        GData ( int , int ,string,GData *, int);
        virtual ~GData();
        GData(const GData& other);
        GData& operator=(const GData& other);
        void set_partition ( GPartitioner *);
        GPartitioner *get_partition();
        int get_ld();
        int get_part_countY();
        int get_part_countX();
        GData & operator ()(int i, int j =0) ;
        GData & get_part(int ,int);
        GHandle *get_handle();
        bool is_memory_allocated();
        void set_memory(MemoryItem*);
        void set_memory(void *,int);
        long get_element_count();
        int get_rows();
        int get_cols();
        byte *get_memory();
        void fill_with(double,double);
        void fill_rows_with ( double start , double scale);
        void print();
        void dump();
        void fill_moler();
        void fill_chol_diag();
        int get_child_index();
        void getCoordination( int &r, int &c, int &d);
        string get_name();
        GData *get_parent();
        int get_level();
        void set_level(int);
        void set_guest(void *);
        void *get_guest();
        void serialize(byte *buf, int &ofs);
        void deserialize(byte *buf, int &ofs);
    protected:

    private:
  int M,N,child_idx,child_cnt,level,key,leading_dim;
        GHandle *handle;
        GData *parent, **children;
        MemoryItem *memory;
        byte *content;
        GPartitioner *partitioner;
        string name;
        void *guest;
};
extern std::vector<GData *> *all_data;
GData* DeserializeData(byte *,int &);
GData* CreateData(byte *,int &);
} // namespace utp
#endif // GDATA_HPP
