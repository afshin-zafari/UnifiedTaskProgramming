#ifndef UNIFIED_ADD_HPP
#define UNIFIED_ADD_HPP
#include "utp.hpp"

class unified_add : public GOperation
{
    public:
        unified_add();
        virtual ~unified_add();
        virtual void run(GTask *) override;
    protected:
    private:
};
void uadd(GData &, GData &, GTask *p=nullptr);
GTask &uadd_t(GData &, GData &, GTask *p=nullptr);
const char *uadd_name ="uadd";
extern unified_add _uadd;
#endif // UNIFIED_ADD_HPP
