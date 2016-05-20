#ifndef UNIFIED_COPY_HPP
#define UNIFIED_COPY_HPP
#include "utp.hpp"

class unified_copy : public GOperation
{
    public:
        unified_copy();
        virtual ~unified_copy();
        virtual void run(GTask *) override;
    protected:
    private:
};
void ucopy(GData &, GData &, GTask *p=nullptr);
GTask &ucopy_t(GData &, GData &, GTask *p=nullptr);
const char *ucopy_name ="ucopy";
extern unified_copy _ucopy;

#endif // UNIFIED_COPY_HPP
