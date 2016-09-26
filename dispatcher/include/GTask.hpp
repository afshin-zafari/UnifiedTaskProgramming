/** \ingroup gen_group Generic Classes
*/

#ifndef GTASK_HPP
#define GTASK_HPP

#include "utp_basic.hpp"
#include "IScheduler.hpp"
#include "sg/superglue.hpp"
#include "GHandle.hpp"


#include <string>
#include <vector>

class GOperation;
class GTask
{
    public:
        string fname;
        Args *args;
        Axs *axs;
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
        void *get_guest();
        void set_guest(void *);
        GOperation *get_operation();
        void set_operation(GOperation*);
        void serialize(byte*,int &);
        void deserialize(byte*,int &);
        int get_key();
        Args * get_args();
        Axs * get_axs();
    protected:
    private:
    GOperation *operation;
    int key;
    GHandle *handle;
    GTask *parent;
    IScheduler *owner;
    double alpha,beta;
    bool trans_a,trans_b,trans_c,left_side,right_side, upper_tr,lower_tr,unit_diagonal;
    void *guest;
};
extern std::vector<GTask*> all_tasks;
GTask *DeserializeTask(byte *buf, int &);
#endif // GTASK_HPP
