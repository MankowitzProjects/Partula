#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "MotorController.h"
#include "SensorController.h"

typedef enum
{
    STATUS_ROBOT_NULL = 0x00000000,
    STATUS_ROBOT_EXPLORING,
    STATUS_ROBOT_DOCKING,
    STATUS_ROBOT_LIGHT_DETECTED,

    STATUS_ROBOT_BUTT
} STATUS_ROBOT;

extern MotorController g_motoCtrl;

class Robot
{
public:
    void init(void);
    void run(void);
    void fin(void);

    Robot(void);
    ~Robot(void);
private:

    bool isInit;


};

Robot::Robot(void)
{
    prt_debug("Creating robot\n");
    isInit = false;
}

Robot::~Robot(void)
{
    fin();
}

void Robot::run(void)
{
    while ()
    {
    }

    g_motoCtrl.setAccVel(80.0, 90.0);

    g_motoCtrl.goForwards(1000);

    g_motoCtrl.turnLeft(1000);

    g_motoCtrl.stop();
    wait(1000);

    g_motoCtrl.turnRight(1000);

    g_motoCtrl.goBackwards(1000);

    g_motoCtrl.stop();

    wait(1000);
}

void Robot::init(void)
{
    if (isInit)
    {
        return;
    }

    g_sensoCtrl.init();
    g_motoCtrl.init();

    isInit = true;
}

void Robot::fin(void)
{
    g_sensoCtrl.fin();
    g_motoCtrl.fin();
}

#endif // ROBOT_H_INCLUDED
