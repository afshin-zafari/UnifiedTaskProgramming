#ifndef ISCHEDULER_HPP
#define ISCHEDULER_HPP

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "utp_basic.hpp"

class GTask;
class GData;
class IScheduler
{
    public:
        IScheduler(int );
        virtual ~IScheduler()=0;
        IScheduler(const IScheduler& other);
        IScheduler& operator=(const IScheduler& other);
        virtual void submitTask(GTask *)=0;
        virtual void runTask(GTask *)=0;
        //virtual void finishedTaskGroup(GTask* t)=0;
        virtual void finishedTask(GTask *)=0;
        unsigned int get_id();
        virtual void finalize()=0;
        virtual int init()=0;
        virtual void allocate_memory(GData*)=0;
        virtual void data_created(GData *)=0;
        virtual void data_partitioned(GData *)=0;
        IScheduler *next,*previous;
        string get_name();
        void set_name(string);
    protected:
    private:
        unsigned int ID;
        string name;
};

#endif // ISCHEDULER_HPP
