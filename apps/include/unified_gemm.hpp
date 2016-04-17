#ifndef UNIFIED_GEMM_HPP
#define UNIFIED_GEMM_HPP

#include "unified_tasks.hpp"
class unified_gemm: public GOperation
{
    public:
        unified_gemm();
        virtual ~unified_gemm();
        virtual void run(GTask *) override;
    protected:
    private:

};

void ugemm(GData &a,GData &b,GData &c,GTask *parent=nullptr);
void ugemm_dec(GData &a,GData &b,GData &c,GTask *parent=nullptr);
GTask &ugemm_t(GData &a,GData &b,GData &c,GTask *parent=nullptr);

const char *ugemm_name="gemm";
extern unified_gemm _ugemm;

#endif // UNIFIED_GEMM_HPP
