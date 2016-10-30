#include "GLog.hpp"
/*----------------------------------------------*/
GLog::ev_record::ev_record(){

  ev=o1=o2=o3=o4=o5=o1t=o2t=o3t=o4t=o5t=0;
  time=0L;
  c=0;

}
/*----------------------------------------------*/
GLog::GLog(){
  cur_record = new ev_record;
  gettimeofday(& start_time,NULL);
}
/*----------------------------------------------*/
GLog::~GLog(){
  printf("Glog destroyed\n");
  glog.flush();
}
/*----------------------------------------------*/
GLog &GLog::operator << (GTask &t){
  cur_record->o1 = t.get_handle()->get_key();
  cur_record->o1t = t_task;
  if ( cur_record->ev == ev_submit )
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
long GLog::toc(){
  timeval end_time;
  gettimeofday(&end_time,NULL );
  return ((end_time.tv_sec-start_time.tv_sec)*1000000+
	   end_time.tv_usec-start_time.tv_usec)/1.0;
}
/*----------------------------------------------*/
GLog &GLog::operator <<(EndLog e){
  pthread_t tid = pthread_self();
  if ( thread_events[tid] == nullptr) {
    thread_events[tid] = new EventList;
  }
  EventList *ev_list=thread_events[tid];
  cur_record->time =toc();
  int pos=distance(thread_events.begin(),thread_events.find(tid));
  cur_record->c = pos;
  ev_list->push_back(cur_record);
  cur_record = new ev_record;
  return *this;
}
/*----------------------------------------------*/
GLog &GLog::operator <<(EventCode e){
  cur_record->ev= e;
  return *this;
}
/*----------------------------------------------*/
void GLog::flush(){
  MapEventListIter it;
  dump_definition();
  FILE *log_file;
  char log_fname[20];
  sprintf(log_fname ,"./log/n%02d_events.csv",me);
  log_file = fopen(log_fname,"w");
  if ( log_file != NULL ){
      fprintf(log_file,"thread,event,obj1,obj2,obj3,obj4,obj5, o1_type, o2_type,o3_type,o4_type,o5_type, time,info\n");

      for ( it = thread_events.begin(); it != thread_events.end();it++){
        EventList &ev_list = *it->second;
        for (uint i=0;i<ev_list.size();i++){
          ev_record e = *ev_list[i];
          fprintf(log_file,"%ld,%d, %d,%d,%d,%d,%d, %d,%d,%d,%d,%d, %ld,'%s'\n",
              (long)e.c,
              e.ev,
              e.o1 ,e.o2 ,e.o3 ,e.o4 ,e.o5 ,
              e.o1t,e.o2t,e.o3t,e.o4t,e.o5t,
              e.time,e.info.c_str());

        }
      }
      fclose(log_file);
  }
}
/*----------------------------------------------*/
void GLog::dump_definition(){
  FILE *def_file, *type_file;
  char def_fname[20],type_fname[20];

  sprintf(def_fname ,"./log/n%02d_def.csv"    ,me);
  sprintf(type_fname,"./log/n%02d_typedef.csv",me);

  def_file=fopen(def_fname,"w");
  if(def_file != NULL){
      fprintf(def_file,"EvenetId,EventName\n");
      fprintf(def_file,"%d,create\n"    ,ev_create);
      fprintf(def_file,"%d,submit\n"    ,ev_submit);
      fprintf(def_file,"%d,partition\n" ,ev_partition);
      fprintf(def_file,"%d,run\n"       ,ev_run);
      fprintf(def_file,"%d,finished\n"  ,ev_finished);
      fprintf(def_file,"%d,inc_ver_st\n",ev_inc_vst);
      fprintf(def_file,"%d,inc_ver_rt\n",ev_inc_vrt);
      fclose(def_file);
  }

  type_file  = fopen(type_fname,"w");
  if(type_file != NULL ){
      fprintf(type_file,"TypeId,TypeName\n");
      fprintf(type_file,"%d,data\n"      ,t_data);
      fprintf(type_file,"%d,task\n"      ,t_task);
      fprintf(type_file,"%d,partition\n" ,t_partition);
      fprintf(type_file,"%d,listener\n"  ,t_listener);
      fclose(type_file);
  }
}


/*----------------------------------------------*/

GLog glog;
