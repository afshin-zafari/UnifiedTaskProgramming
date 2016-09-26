#ifndef GHANDLE_HPP
#define GHANDLE_HPP

typedef unsigned long GHandleKey;
typedef unsigned char byte;
class GHandle
{
    public:
        GHandle();
        virtual ~GHandle();
        GHandle(const GHandle& other);
        GHandle& operator=(const GHandle& other);
        GHandleKey get_key();
        static GHandleKey lastKey;
        void serialize(byte *,int &);
        void deserialize(byte*,int &);
    protected:
    private:
    GHandleKey key;
};

#endif // GHANDLE_HPP
