#ifndef GOPERATION_HPP
#define GOPERATION_HPP
#include "GTask.hpp"
#include <vector>
class GOperation
{
    public:
        GOperation(){}
        ~GOperation(){}
        GOperation(const GOperation& other);
        GOperation& operator=(const GOperation& other);
        inline virtual void run(GTask *){}
        void serialize(byte *buf, int &ofs);
        void deserialize(byte *buf, int &ofs);
        string name;
        int  get_key();
    protected:
        int key;
    private:
};
extern std::vector<GOperation*> all_operations;
GOperation *DeserializeOperation(byte *buf, int &);
#endif // GOPERATION_HPP
