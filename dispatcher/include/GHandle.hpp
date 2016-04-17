#ifndef GHANDLE_HPP
#define GHANDLE_HPP

typedef unsigned long GHandleKey;
class GHandle
{
    public:
        GHandle();
        virtual ~GHandle();
        GHandle(const GHandle& other);
        GHandle& operator=(const GHandle& other);
        GHandleKey get_key();
        static GHandleKey lastKey;
    protected:
    private:
    GHandleKey key;
};

#endif // GHANDLE_HPP
