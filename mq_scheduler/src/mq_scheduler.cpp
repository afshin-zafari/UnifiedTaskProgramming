#include "mq_scheduler.hpp"

namespace mq{
  boost::asio::io_service *ioService;
  AsioHandler *handler;
  AMQP::Connection *connection;
  AMQP::Channel *channel,*channel_recv;
  boost::asio::deadline_timer *t;
  MQWrapper *mq_wrapper;
  
}

/*----------------------------------------------------------------------------------------*/
mq::MQ::MQ(MQWrapper *mqw){
  mq_wrapper=mqw;
  std::cout <<"1\n";
  ioService = new boost::asio::io_service();
  handler = new AsioHandler(*ioService);
  std::cout <<"2\n";
  handler->connect("130.238.29.221", 5672);

  connection = new AMQP::Connection (handler, AMQP::Login("afshin", "afshin"), "/");
  channel = new AMQP::Channel (connection);
    
  channel_recv = new AMQP::Channel (connection);
  channel_recv->declareQueue("hello");
  channel_recv->consume("hello",AMQP::noack).onReceived(receive);
    

  t= new boost::asio::deadline_timer(*ioService, boost::posix_time::millisec(1000));
  std::cout <<"3\n";

  channel->onReady(ready);
    
}
/*----------------------------------------------------------------------------------------*/
mq::MQ::~MQ(){
  delete t;
  delete channel_recv;
  delete channel;
  delete connection;
  delete handler;
  delete ioService;
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
  byte *buf = (byte *)message.body();
  int ofs=0,tag;
  paste(buf,ofs,&tag);
  ofs = message.bodySize()- ofs;
  Message * msg = new Message (buf,ofs,tag);
  std::cout <<" [x] Received command with tag: "<< tag << "size: " << ofs << std::endl;
  mq::mq_wrapper->run_rpc(msg);
  delete msg;
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send(const char *msg){
  mq::channel->publish("", "recv", msg);
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send_buffer(const char *buf,int n){
  AMQP::Envelope e(buf,n);
  mq::channel->publish("", "recv", e);
}
/*----------------------------------------------------------------------------------------*/
