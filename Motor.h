#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include "conf.h"

using namespace std;

class Motor
{
public:
    Motor(void);

    void init(int index, CPhidgetMotorControlHandle *pMotorCtrlHandle, bool bIsReversed);
    void fin(void);

    inline void  act(float acceleration, float velocity);
    inline void  setAcc(float acceleration);
    inline void  setVel(float velocity);

    inline float getVel(void);
    inline float getAcc(void);

    inline void  stop(void);

    inline bool  isReversed(void) {return bIsReversed;}
    
    

private:

    CPhidgetMotorControlHandle *pMotorCtrlHandle;   /**< The motor control handle */

    int index;      /**< The index of the motor */

    float vel;      /**< The velocity */
    float acc;      /**< The acceleration */

    bool  bIsReversed;
};


/** \brief Get the current velocity
 *
 * \param void
 * \return float
 *
 */
inline float Motor::getVel(void) {return vel;}

/** \brief Get the current acceleration
 *
 * \param void
 * \return float
 *
 */
inline float Motor::getAcc(void) {return acc;}

/** \brief stop the motor
 *
 * \param void
 * \return void
 *
 */
void Motor::stop(void)
{
    // stop the motor
    act(0.0, 0.0);
}

/** \brief
 *
 * \param acceleration float
 * \param velocity float
 * \return void
 *
 */
inline void Motor::act(float acceleration, float velocity)
{
    setAcc(acceleration);
    setVel(velocity);
}


inline void Motor::setAcc(float acceleration)
{
    if (acceleration != this->acc)
    {
        #if (DEBUG_MODE_MOTOR)
        printf("Motor::setAcc - set motor %d acc to %f\n", index, acceleration);
        #endif

        this->acc = acceleration;

        if (!bIsReversed)
        {
            CPhidgetMotorControl_setAcceleration(*pMotorCtrlHandle, index, acceleration);
        }
        else
        {
            CPhidgetMotorControl_setAcceleration(*pMotorCtrlHandle, index, -acceleration);
        }
    }
}

inline void Motor::setVel(float velocity)
{
    if (velocity != this->vel)
    {
        #if (DEBUG_MODE_MOTOR)
        printf("Motor::setAcc - set motor %d vel to %f\n", index, velocity);
        #endif

        this->vel = velocity;

        if (!bIsReversed)
        {
            CPhidgetMotorControl_setVelocity(*pMotorCtrlHandle, index, velocity);
        }

        else
        {
            CPhidgetMotorControl_setVelocity(*pMotorCtrlHandle, index, -velocity);
        }
    }
}

#endif // MOTOR_H_INCLUDED
