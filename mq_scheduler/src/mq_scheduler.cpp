#include "mq_scheduler.hpp"
#include "config.hpp"

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
  std::cout << config.mq_ip << " "
	    << config.mq_send << " "
	    << config.mq_recv << " "
	    << config.mq_name << " "
	    << config.mq_pass << std::endl;
  
  handler->connect(config.mq_ip, 5672);//"130.238.29.221"

  connection = new AMQP::Connection (handler, AMQP::Login(config.mq_name,config.mq_pass), "/"); //"afshin", "afshin"
  channel = new AMQP::Channel (connection);
    
  channel_recv = new AMQP::Channel (connection);
  channel_recv->declareQueue(config.mq_recv);//"hello"
  channel_recv->consume(config.mq_recv,AMQP::noack).onReceived(receive);//"hello"
    

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
  while (!mq::handler->connected()) ;
  if(mq::handler->connected())
    {
      //mq::channel->publish("", "hello", "Hello World!by Afshin. Inside a class.");
      std::cout << " [x] Connection established " << std::endl;	
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
  for(int i=0;i<ofs;i++){
    printf("%3.3d ",buf[i]);
    if(i and i %3 ==0)
      printf("\n");
  }
  mq::mq_wrapper->run_rpc(msg);
  delete msg;
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send(const char *msg){
  mq::channel->publish("", config.mq_send, msg);
}
/*----------------------------------------------------------------------------------------*/
void mq::MQ::send_buffer(const char *buf,int n){
  if (!mq::handler->connected()) {
    std::cout << "MQ not connected.\n";
    return;
  }
  std::cout << "sent to " << config.mq_send << " " << n <<" bytes" << std::endl;
  AMQP::Envelope e(buf,n);
  mq::channel->publish("", config.mq_send, e);
}
/*----------------------------------------------------------------------------------------*/
