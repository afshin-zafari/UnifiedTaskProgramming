#ifndef GHANDLE_HPP
#define GHANDLE_HPP
#include <string>

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
        operator std::string() const;
    protected:
    private:
    GHandleKey key;
};

#endif // GHANDLE_HPP
