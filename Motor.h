#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#include "conf.h"

class Motor
{
public:
    Motor(void);

    void init(int index, CPhidgetMotorControlHandle *pMotoControl, bool bReverse);
    void fin(void);

    inline void act(float acceleration, float velocity);
    inline void applyAcceleration(float acceleration);
    inline void applyVelocity(float velocity);

    inline float getVelocity(void);

    inline float getAcceleration(void);

    void spin(void);

    void stop(void);

    void bSetReverse(bool bIsReversed) {this->bIsReversed = bIsReversed;}

private:

    CPhidgetMotorControlHandle *pMotoControl;   /**< The motor control handle */

    int index;          /**< The index of the motor */

    float velocity;     /**< The velocity */
    float acceleration; /**< The acceleration */

    bool  bIsReversed;   /**< If the motor is Reversed */
};

/** \brief The default constructor of motor
 *
 * \param void
 *
 */
Motor::Motor(void) :
    pMotoControl(NULL),
    index(0),
    velocity(0.0),
    acceleration(0.0),
    bIsReversed(false)
{
    ;
}

/** \brief This function will initialize all the parameters
 *
 * \param index int     The index number of the motor
 * \param pMotoControl CPhidgetMotorControlHandle*      The handle of the phidget motor
 * \return void
 *
 */
void Motor::init(int index, CPhidgetMotorControlHandle *pMotoControl, bool bReverse)
{
    // set the default value
    velocity          = 0.0;
    acceleration      = 0.0;
    this->index       = index;
    this->bIsReversed = bReverse;

    // set the handle pointer value
    this->pMotoControl = pMotoControl;

    // stop the motor
    stop();
    printf("SingleMotor::init, index=%d\n", this->index);
}

/** \brief Stop the motor and clean up everything allocated
 *
 * \param void
 * \return void
 *
 */
void Motor::fin(void)
{
    // stop the motor
    stop();

    // set the handle pointer value
    this->pMotoControl = NULL;
}

/** \brief Get the current velocity
 *
 * \param void
 * \return float
 *
 */
inline float Motor::getVelocity(void) {return velocity;}

/** \brief Get the current acceleration
 *
 * \param void
 * \return float
 *
 */
inline float Motor::getAcceleration(void) {return acceleration;}

/** \brief Make the motor spin
 *
 * \param void
 * \return void
 *
 */
void Motor::spin(void)
{
    // excute the function provide by phidget
    act(acceleration, velocity);
}

/** \brief stop down the motor
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
    #if (1 == DEBUG_MODE)
    printf("Motor::act - set motor %d velocity to %f\n", index, velocity);
    #endif

    applyAcceleration(-acceleration);
    applyVelocity(-velocity);
}

inline void Motor::applyAcceleration(float acceleration)
{
    if (acceleration != this->acceleration)
    {
        this->acceleration = acceleration;
        CPhidgetMotorControl_setAcceleration(*pMotoControl, index, acceleration);

        if (!bIsReversed)
        {
            CPhidgetMotorControl_setAcceleration(*pMotoControl, index, acceleration);
        }
        else
        {
            CPhidgetMotorControl_setAcceleration(*pMotoControl, index, -acceleration);
        }
    }
}

inline void Motor::applyVelocity(float velocity)
{
    if (velocity != this->velocity)
    {
        this->velocity = velocity;

        if (!bIsReversed)
        {
            CPhidgetMotorControl_setVelocity(*pMotoControl, index, velocity);
        }
        else
        {
            CPhidgetMotorControl_setVelocity(*pMotoControl, index, -velocity);
        }
    }
}

#endif // MOTOR_H_INCLUDED
