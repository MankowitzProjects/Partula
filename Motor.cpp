#include "Motor.h"

/** \brief The default constructor of motor
 *
 * \param void
 *
 */
Motor::Motor(void) :
    pMotorCtrlHandle(NULL),
    index(0),
    vel(0.0),
    acc(0.0),
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
void Motor::init(int index, CPhidgetMotorControlHandle *pMotorCtrlHandle, bool bIsReversed)
{
    // set the default value
    vel = 0.0;
    acc = 0.0;

    this->index       = index;
    this->bIsReversed = bIsReversed;

    // set the handle pointer value
    this->pMotorCtrlHandle = pMotorCtrlHandle;

    // stop the motor
    stop();

    printf("Motor::init, index = %d\n", this->index);
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
    this->pMotorCtrlHandle = NULL;
}

