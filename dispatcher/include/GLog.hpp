#ifndef __GLOG_HPP_
#define __GLOG_HPP_
#include <string>
#include <vector>
#include <map>
#include "GData.hpp"
#include "GTask.hpp"

using namespace std;
enum {
    endlog=-1,
    ev_create,
    ev_submit,
    ev_partition,
    ev_count
};
enum {
    t_data,
    t_task,
    t_partition,
    t_listener,
    t_count
};
class GLog{
private:
    struct ev_record{
        long ev,o1,o2,o3,o4,o5;
        int o1t,o2t,o3t,o4t,o5t;
        long time;
        string info;
    };
    typedef vector<ev_record *> EventList;
    map<long,EventList*> thread_events;
    ev_record *cur_record;
public:
  GLog();
  GLog &operator << (GTask &t);
  GLog &operator << (GData &d);
  GLog &operator <<(int i);
  void flush();
  void dump_definition();
};
extern GLog glog;

#endif //__GLOG_HPP_
