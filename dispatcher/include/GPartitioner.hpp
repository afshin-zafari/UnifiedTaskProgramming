#ifndef GPARTITIONER_HPP
#define GPARTITIONER_HPP

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
    protected:
        GPartitioner  *next;
    private:
        bool comm,ownership,alloc;
};

#endif // GPARTITIONER_HPP
