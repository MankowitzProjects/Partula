#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED

#include "conf.h"

#include "MotorController.h"
#include "SensorController.h"

//#include "Event.h"

using namespace std;

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

#endif // ROBOT_H_INCLUDED
