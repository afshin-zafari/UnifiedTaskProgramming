#ifndef GOPERATION_HPP
#define GOPERATION_HPP
#include "GTask.hpp"

class GOperation
{
    public:
        GOperation();
        virtual ~GOperation();
        GOperation(const GOperation& other);
        GOperation& operator=(const GOperation& other);
        virtual void run(GTask *){}
        string name;
    protected:
    private:
};

#endif // GOPERATION_HPP
