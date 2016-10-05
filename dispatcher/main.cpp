#include <iostream>
#include "Dispatcher.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  Dispatcher d(argc,argv);
  cout << "Hello world!" << endl;
  return 0;
}
