#include <iostream>
#include "utp.hpp"

using namespace std;



int main(int argc, char *argv[])
{
    cout << "Starts\n";

    utp::utp_initialize(argc,argv);

    test_DT_FORK_SG_SPU();

    utp::utp_finalize();
    cout << "Finished\n";

    return 0;
}
