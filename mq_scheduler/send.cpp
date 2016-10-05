#include "mq_scheduler.hpp"

int main(void)
{
  mq::MQ mq;
  mq.send(" method sentence.");
  char msg[20]="Buffer Msg.";
  mq.send_buffer(msg,10);
  mq::ioService.run();
  return 0;
}
