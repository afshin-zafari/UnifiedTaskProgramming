#ifndef UNIFIED_DOT_HPP
#define UNIFIED_DOT_HPP
#include "utp.hpp"

class unified_dot : public GOperation
{
    public:
        unified_dot();
        virtual ~unified_dot();
        virtual void run(GTask *) override;
    protected:
    private:
};
void udot(GData &, GData &, GData &,GTask *p=nullptr);
GTask &udot_t(GData &, GData &,GData &, GTask *p=nullptr);
const char *udot_name ="udot";
extern unified_dot _udot;
#endif // UNIFIED_DOT_HPP
