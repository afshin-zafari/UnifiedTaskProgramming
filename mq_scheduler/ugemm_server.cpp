#include "Dispatcher.hpp"
int main ( int argc, char *argv[])
{
  utp_initialize(argc,argv);
  Dispatcher * dis = get_dispatcher();
  if ( dis != nullptr )
    dis->set_mq_mode(true);
  utp_finialize();
}
