#include "GLog.hpp"
/*----------------------------------------------*/
GLog::GLog(){
  cur_record = new ev_record;
  thread_events[0]=new EventList;
}
/*----------------------------------------------*/
GLog &GLog::operator << (GTask &t){
  cur_record->o1 = t.get_handle()->get_key();
  cur_record->o1t = t_task;
  if ( cur_record->ev == ev_create )
    cur_record->info=t.get_name();
  return *this;
}
/*----------------------------------------------*/
GLog &GLog::operator << (GData &d){
  cur_record->o1 = d.get_handle()->get_key();
  cur_record->o1t = t_data;
  if ( cur_record->ev == ev_create )
    cur_record->info=d.get_name();
  return *this;
}
/*----------------------------------------------*/
GLog &GLog::operator <<(int i){
  if ( i == endlog){
    EventList *ev_list=thread_events[0];
    ev_list->push_back(cur_record);
    cur_record = new ev_record;
    return *this;
  }
  cur_record->ev= i;
  return *this;
}
/*----------------------------------------------*/
void GLog::flush(){
  EventList &ev_list = *thread_events[0];
  for (uint i=0;i<ev_list.size();i++){
    ev_record e = *ev_list[i];
    cout << e.ev <<','
	 << e.o1 << ','
	 << e.info << endl;
  }
}
/*----------------------------------------------*/
void GLog::dump_definition(){

}
/*----------------------------------------------*/

GLog glog;
