#include "mq_scheduler.hpp"

namespace mq{
  boost::asio::io_service ioService;
  AsioHandler *handler;
  AMQP::Connection *connection;
  AMQP::Channel *channel,*channel_recv;
  boost::asio::deadline_timer *t;  
  
}

/*----------------------------------------------------------------------------------------*/
mq::MQ::MQ(){
  handler = new AsioHandler(ioService);
  handler->connect("130.238.28.158", 5672);

  connection = new AMQP::Connection (handler, AMQP::Login("afshin", "afshin"), "/");
  channel = new AMQP::Channel (connection);
    
  channel_recv = new AMQP::Channel (connection);
  channel_recv->declareQueue("recv");
  channel_recv->consume("recv",AMQP::noack).onReceived(receive);
    

  t= new boost::asio::deadline_timer(ioService, boost::posix_time::millisec(1000));
  channel->onReady(ready);
    
}
/*----------------------------------------------------------------------------------------*/
mq::MQ::~MQ(){
  delete t;
  delete channel_recv;
  delete channel;
  delete connection;
  delete handler;    
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::ready(){
  if(mq::handler->connected())
    {
      mq::channel->publish("", "hello", "Hello World!by Afshin. Inside a class.");
      std::cout << " [x] Sent 'Hello World!'" << std::endl;	
    }  
}
/*----------------------------------------------------------------------------------------*/
 void mq::MQ::receive(const AMQP::Message &message,uint64_t deliveryTag,bool redeliver)
{
  std::cout <<" [x] Received "<<message.message()<< "size: " << message.bodySize() << std::endl;
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send(const char *msg){
  mq::channel->publish("", "hello", msg);
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send_buffer(const char *buf,int n){
  AMQP::Envelope e(buf,n);
  mq::channel->publish("", "hello", e);
}
/*----------------------------------------------------------------------------------------*/
