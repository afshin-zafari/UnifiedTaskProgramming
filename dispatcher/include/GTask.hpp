#ifndef GTASK_HPP
#define GTASK_HPP

#include "utp_basic.hpp"
#include "IScheduler.hpp"
#include "sg/superglue.hpp"
#include "GHandle.hpp"

#include <string>
class GTask
{
    public:
        string fname;
        Args *args;
        int task_count;
        GTask();
        GTask(string,Args*,FUT);
        virtual ~GTask();
        GTask(const GTask& other);
        GTask& operator=(const GTask& other);
        IScheduler *get_owner();
        void set_owner(IScheduler *);
        GTask *get_parent();
        void set_parent(GTask *);
        GHandle *get_handle();
        GTask &Beta(double v);
        GTask &Alpha(double );
        GTask &TransA(bool);
        GTask &TransB(bool);
        GTask &TransC(bool);
        GTask &LeftSide(bool);
        GTask &RightSide(bool);
        GTask &UpperTriangle(bool);
        GTask &LowerTriangle(bool);
        GTask &UnitDiagonal(bool);

        double get_beta();
        double get_alpha();
        bool get_trans_a();
        bool get_trans_b();
        bool get_trans_c();
        bool get_left_side();
        bool get_right_side();
        bool get_upper_triangle();
        bool get_lower_triangle();
        bool get_unit_diagonal();
        string get_name();
    protected:
    private:
    GHandle *handle;
    GTask *parent;
    IScheduler *owner;
    double alpha,beta;
    bool trans_a,trans_b,trans_c,left_side,right_side, upper_tr,lower_tr,unit_diagonal;
};

#endif // GTASK_HPP
