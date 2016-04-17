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
    protected:
         GPartitioner  *next;
    private:
};

#endif // GPARTITIONER_HPP
