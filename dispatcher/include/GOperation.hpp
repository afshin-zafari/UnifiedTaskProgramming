#ifndef GOPERATION_HPP
#define GOPERATION_HPP
#include "GTask.hpp"

class GOperation
{
    public:
  GOperation(){}
  ~GOperation(){}
        GOperation(const GOperation& other);
        GOperation& operator=(const GOperation& other);
        inline virtual void run(GTask *){}
        string name;
    protected:
    private:
};

#endif // GOPERATION_HPP
