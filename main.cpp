#include <iostream>

#include "Robot.h"

using namespace std;

pthread_t thread1,scanthread;

int main()
{
    Robot partula;

    partula.init();
    partula.run();
    partula.fin();

    return 0;
}
