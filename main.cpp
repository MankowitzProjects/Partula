#include <iostream>

#include "Robot.h"


using namespace std;

MotorController  g_motoCtrl;
SensorController g_sensoCtrl;

int main()
{
    cout << "Hello world!" << endl;

    Robot partula;
    partula.init();
    partula.run();
    partula.fin();

    return 0;
}
