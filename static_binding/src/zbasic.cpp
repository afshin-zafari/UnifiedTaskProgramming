#include <sys/time.h>
//typedef unsigned long TimeUnit;
typedef double TimeUnit;

TimeUnit UserTime() {
  timeval tv;
  TimeUnit  unit=1000000.0;
  gettimeofday(&tv, 0);
  return (tv.tv_sec*unit+tv.tv_usec);
}
