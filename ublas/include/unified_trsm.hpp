#ifndef UNIFIED_TRSM_HPP
#define UNIFIED_TRSM_HPP
#include "utp.hpp"

class unified_trsm : public GOperation
{
    public:
        unified_trsm();
        virtual ~unified_trsm();
        virtual void run(GTask *) override;
    protected:
    private:
};
void utrsm(GData &, GData &, GTask *p=nullptr);
GTask &utrsm_t(GData &, GData &, GTask *p=nullptr);
const char *utrsm_name ="utrsm";
extern unified_trsm _utrsm;
#endif // UNIFIED_TRSM_HPP
