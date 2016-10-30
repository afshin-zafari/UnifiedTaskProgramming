#include <sys/time.h>
typedef unsigned long TimeUnit;

TimeUnit UserTime() {
  timeval tv;
  long unit=1000000;
  gettimeofday(&tv, 0);
  return (tv.tv_sec*unit+tv.tv_usec/unit);
}
