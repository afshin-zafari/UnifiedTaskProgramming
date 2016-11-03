#ifndef __GLOG_HPP_
#define __GLOG_HPP_

#include <string>
#include <vector>
#include <map>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

#include "GData.hpp"
#include "GTask.hpp"


#define EnableOption(x) struct x : public ConfigBase { typedef Enable LogOutput;}
#define DisableOption(x) struct x : public ConfigBase { typedef Disable LogOutput;}

template <typename T,typename U = typename T::LogOutput>class LogConfig;

template <typename T>
class LogConfig<T,typename T::Enable>{
public:
    static void test(){std::cout << "A\n";}
};
template <typename T>
class LogConfig<T,typename T::Disable >{
public:
    static void test(){}
};
template <typename Config>
class Log : public LogConfig<Config>{
};
template <typename T, typename U= typename T::LogOutput, typename V=typename T::LogCategory> class Log2;
template < typename T,typename U>
class Log2<T,U,typename T::Enable>: public LogConfig<T,U>{
public:
    static void test(){std::cout << "B\n";}
};
template <typename T, typename U>
class Log2<T,U,typename T::Disable >: public LogConfig<T,U>{
public:
    static void test(){}
};


class ConfigBase{
    public:
    struct Enable{};
    struct Disable{};
};



using namespace std;
typedef enum EventCode{
  ev_starting_number=10,
  ev_create,
  ev_submit,
  ev_partition,
  ev_run,
  ev_finished,
  ev_inc_vst,
  ev_inc_vrt,
  ev_count
}EventCode;
enum {
    t_starting_number=10,
    t_data,
    t_task,
    t_partition,
    t_listener,
    t_count
};
typedef struct EndLog{int i;}EndLog;
EndLog endlog;
class GLog{
private:
    struct ev_record{
        int ev,o1,o2,o3,o4,o5;
        int o1t,o2t,o3t,o4t,o5t;
        long  time;
        string info;
        pthread_t c;
      ev_record();
    };
    typedef vector<ev_record *> EventList;
    typedef map<pthread_t,EventList*> MapEventList;
    typedef MapEventList::iterator MapEventListIter;
    MapEventList thread_events;
    ev_record *cur_record;
    timeval start_time;
public:
  GLog();
  ~GLog();
  GLog &operator << (GTask &);
  GLog &operator << (GData &);
  GLog &operator << (EventCode);
  GLog &operator << (EndLog );
  void flush();
  void dump_definition();
  long toc();
};
extern GLog glog;
extern int me;
#endif //__GLOG_HPP_
