#ifndef MOTORCONTROLLER_H_INCLUDED
#define MOTORCONTROLLER_H_INCLUDED

#include "conf.h"
#include "Motor.h"

//**************** define the motor handlers ****************


class MotorController
{
public:
    MotorController(void);
    void init(void);
    void fin(void);

    void setVelocity(float velocity) {setVelocity(velocity, velocity);}
    void setAcceleration(float acceleration) {setAcceleration(acceleration, acceleration);}

    void setVelocity(float velocityLeft, float velocityRight);
    void setAcceleration(float accelerationLeft, float accelerationRight);

    // Temp functions, you must use setAccVel before make the movements~!

    float acc;
    float vel;
    void setAccVel(float acc, float vel) {this->acc = acc; this->vel = vel;};

    void goForwards(int iTime);
    void goBackwards(int iTime);
    void turnLeft(int iTime);
    void turnRight(int iTime);
    void stop(void);

    void go(float acc, float vel, int iTime);
    void turn(float acc, float vel, int iTime);

    Motor motorLeft;    /**< The left side motor */
    Motor motorRight;   /**< The right side motor */

private:

    CPhidgetMotorControlHandle motoControl; /**< Motor control handle */

    int  regHanlders(void);
    void clrHandlers(void);
};

MotorController::MotorController(void)
{
    ;
}

/** \brief set the velocity for each motor
 *
 * \param velocityLeft float
 * \param velocityRight float
 * \return void
 *
 */
void MotorController::setVelocity(float velocityLeft, float velocityRight)
{
    motorLeft.applyVelocity(velocityLeft);
    motorRight.applyVelocity(velocityRight);
}

/** \brief set the acceleration for each motor
 *
 * \param accLeft float
 * \param accRight floatapply
 * \return void
 *
 */
void MotorController::setAcceleration(float accLeft, float accRight)
{
    motorLeft.applyAcceleration(accLeft);
    motorRight.applyAcceleration(accRight);
}

/** \brief Initialize the motor controller class
 *
 * \param void
 * \return void
 *
 */
void MotorController::init(void)
{
    printf("Motor::init\n");

    // register the motor handlers
    regHanlders();

    // init the two motorsfloat
    motorLeft.init( INDEX_MOTOR_LEFT,  &motoControl, false);
    motorRight.init(INDEX_MOTOR_RIGHT, &motoControl, false);
}

/** \brief Finalize and clear everything, shut down the motors, clear the registered handles
 *
 * \param void
 * \return void
 *
 */
void MotorController::fin(void)
{
    printf("MotorController::fin, finalize the motors\n");

    // execute the fin functions of the two motors to stop them
    motorLeft.fin();
    motorRight.fin();

    // clean all the rest registered handlers
    clrHandlers();
}


int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorAttachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorDetachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}


int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorInputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int index, int state)
{
    #if (DEBUG_MODE_LOW)
	printf("Input %d > state: %d\n", index, state);
	#endif
	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorVelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int index, double value)
{
    #if (DEBUG_MODE_LOW)
	printf("Motor %d > Current Speed: %f\n", index, value);
	#endif
	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
MotorCurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int index, double value)
{
    #if (DEBUG_MODE_LOW)
	printf("Motor: %d > Current Draw: %f\n", index, value);
	#endif
	return 0;
}

int MotorDisplayProperties(CPhidgetMotorControlHandle phid)
{
	int serialNo, version, numInputs, numMotors;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetMotorControl_getInputCount(phid, &numInputs);
	CPhidgetMotorControl_getMotorCount(phid, &numMotors);

    #if (DEBUG_MODE_LOW)
	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Inputs: %d\n# Motors: %d\n", numInputs, numMotors);
    #endif

	return 0;
}

void MotorController::clrHandlers(void)
{
    printf("MotorController::clrHandlers\n");
	CPhidget_close( (CPhidgetHandle)motoControl);
	CPhidget_delete((CPhidgetHandle)motoControl);
}

int MotorController::regHanlders(void)
{
    printf("MotorController::regHanlders\n");
	int result;
	const char *err;

	printf("Motor::registerHanlders\n");

	//Declare a motor control handle
	motoControl = 0;

	//create the motor control object
	CPhidgetMotorControl_create(&motoControl);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)motoControl, MotorAttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)motoControl, MotorDetachHandler, NULL);
	CPhidget_set_OnError_Handler( (CPhidgetHandle)motoControl, MotorErrorHandler,  NULL);

	//Registers a callback that will run if an input changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnInputChange_Handler(motoControl, MotorInputChangeHandler, NULL);

	//Registers a callback that will run if a motor changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnVelocityChange_Handler(motoControl, MotorVelocityChangeHandler, NULL);

	//Registers a callback that will run if the current draw changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnCurrentChange_Handler(motoControl, MotorCurrentChangeHandler, NULL);

	//open the motor control for device connections
	CPhidget_open((CPhidgetHandle)motoControl, -1);

	//get the program to wait for a motor control device to be attached
	printf("Waiting for MotorControl to be attached....");
	if ((result = CPhidget_waitForAttachment((CPhidgetHandle)motoControl, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached motor control device
	MotorDisplayProperties(motoControl);

	//all done, exit
	return 0;
}

/** \brief Drive the vehicle go forward or just performe the new values of velocity and acceleration
 *
 * \param void
 * \return void
 *
 */
void MotorController::goForwards(int iTime)
{
    prt_debug("MotorController::goForwards\n");
    go(acc, vel, iTime);
}

/** \brief Drive the vehicle go backward
 *
 * \param void
 * \return void
 *
 */
void MotorController::goBackwards(int iTime)
{
    prt_debug("MotorController::goBackwards\n");
    go(-acc, -vel, iTime);
}

void MotorController::turnLeft(int iTime)
{
    prt_debug("MotorController::turnLeft\n");

    turn(-acc, -vel, iTime);
}

void MotorController::turnRight(int iTime)
{
    prt_debug("MotorController::turnRight\n");

    turn(acc, vel, iTime);
}

void MotorController::stop(void)
{
    prt_debug("MotorController::stop\n");

    go(0.0, 0.0, 0);
}

void MotorController::go(float acc, float vel, int iTime)
{
    motorLeft.act(acc, vel);
    motorRight.act(acc, vel);

    wait(iTime);
}

void MotorController::turn(float acc, float vel, int iTime)
{
    motorLeft.act(-acc, -vel);
    motorRight.act(acc, vel);

    wait(iTime);
}

#endif // MOTORCONTROLLER_H_INCLUDED
