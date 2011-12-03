#ifndef MOTORCONTROLLER_H_INCLUDED
#define MOTORCONTROLLER_H_INCLUDED

#include "conf.h"
#include "Motor.h"

using namespace std;



class MotorController
{
public:

    MotorController(void);
    void init(void);
    void fin(void);

    inline void setVelLeft(float velLeft);
    inline void setAccLeft(float accLeft);
    inline void setVelRight(float velRight);
    inline void setAccRight(float accRight);

    inline void setAcc(float acc);
    inline void setVel(float vel);

    inline void setAcc(float accLeft, float accRight);
    inline void setVel(float velLeft, float velRight);

    inline void stop(void);

    Motor motorLeft;    /**< The left  side motor */
    Motor motorRight;   /**< The right side motor */
  
  
private:

    CPhidgetMotorControlHandle motorCtrlHandle; /**< Motor control handle */

    int  regHanlders(void);
    void clrHandlers(void);
};

inline void MotorController::setVelLeft(float velLeft)
{
    motorLeft.setVel(velLeft);
}

inline void MotorController::setAccLeft(float accLeft)
{
    motorLeft.setAcc(accLeft);
}

inline void MotorController::setVelRight(float velRight)
{
    motorRight.setVel(velRight);
}

inline void MotorController::setAccRight(float accRight)
{
    motorRight.setAcc(accRight);
}

inline void MotorController::setAcc(float accLeft, float accRight)
{
    setAccLeft(accLeft);
    setAccRight(accRight);
}
inline void MotorController::setVel(float velLeft, float velRight)
{
    setVelLeft(velLeft);
    setVelRight(velRight);
}

inline void MotorController::setAcc(float acc)
{
    setAcc(acc, acc);
}

inline void MotorController::setVel(float vel)
{
    setVel(vel, vel);
}

inline void MotorController::stop(void)
{
    setAcc(0.0);
    setVel(0.0);
}

#endif // MOTORCONTROLLER_H_INCLUDED
