#ifndef MQ_SCHEDULER_HPP
#define MQ_SCHEDULER_HPP
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "asiohandler.h"
#include "amqpcpp.h"
#include "IScheduler.hpp"

namespace mq{
  extern boost::asio::io_service *ioService;
  extern AsioHandler *handler;
  extern AMQP::Connection *connection;
  extern AMQP::Channel *channel,*channel_recv;
  extern boost::asio::deadline_timer *t;  
  
  class MQ{//:public IScheduler
  public:
    MQ();
    ~MQ();
    static void ready();
    static void receive(const AMQP::Message &message,uint64_t deliveryTag,bool redeliver);
    void send(const char *msg);
    void send_buffer(const char *buf,int n);
  };
} // end namespace mq
#endif //MQ_SCHEDULER_HPP
