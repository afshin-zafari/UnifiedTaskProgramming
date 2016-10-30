
#include <iostream>
#include "utp.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
using namespace std;

int main()
{
    cout << "Starts\n";
    test_DT_FORK_SG_SPU();

    return 0;
}

