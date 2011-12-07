#ifndef SERVOCONTROLLER_H_INCLUDED
#define SERVOCONTROLLER_H_INCLUDED

#include "ServoMotor.h"

class ServoController
{
public:
    //ServoController(void);
    //~ServoController(void);

    void init(void);
    void fin(void);

    double getPos(void);
    void setPos(double pos);

private:
    CPhidgetAdvancedServoHandle srvCtrlHandle;

    double curPos;

    void setupServo(void);
    void regHandlers(void);
    void clrHandlers(void);
};

#endif // SERVOCONTROLLER_H_INCLUDED
