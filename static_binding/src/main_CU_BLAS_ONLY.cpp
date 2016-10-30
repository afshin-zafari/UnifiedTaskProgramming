#include <iostream>
#include "utp.hpp"
#include "ugemm.hpp"
#include "utrsm.hpp"
using namespace std;

int main()
{
    cout << "Starts\n";
    test_cuBLAS_ONLY();

    return 0;
}


