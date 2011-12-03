#include "MotorController.h"

MotorController g_motorCtrl;

MotorController::MotorController(void)
{
    ;
}

/** \brief Initialize the motor controller class
 *
 * \param void
 * \return void
 *
 */
void MotorController::init(void)
{
    printf("MotorController::init\n");

    // register the motor handlers
    regHanlders();

    // init the two motorsfloat
    motorLeft.init( INDEX_MOTOR_LEFT,  &motorCtrlHandle, false);
    motorRight.init(INDEX_MOTOR_RIGHT, &motorCtrlHandle, true);

    printf("MotorController::init - DONE~!\n");
}

/** \brief Finalize and clear everything, shut down the motors, clear the registered handles
 *
 * \param void
 * \return void
 *
 */
void MotorController::fin(void)
{
    printf("MotorController::fin\n");

    // execute the fin functions of the two motors to stop them
    motorLeft.fin();
    motorRight.fin();

    // clean all the rest registered handlers
    clrHandlers();

    printf("MotorController::fin - DONE~!\n");
}



//******************************* Handlers ********************************

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
	CPhidget_close( (CPhidgetHandle)motorCtrlHandle);
	CPhidget_delete((CPhidgetHandle)motorCtrlHandle);
}

int MotorController::regHanlders(void)
{
    printf("MotorController::regHandlers\n");
	int result;
	const char *err;

	//Declare a motor control handle
	motorCtrlHandle = 0;

	//create the motor control object
	CPhidgetMotorControl_create(&motorCtrlHandle);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)motorCtrlHandle, MotorAttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)motorCtrlHandle, MotorDetachHandler, NULL);
	CPhidget_set_OnError_Handler( (CPhidgetHandle)motorCtrlHandle, MotorErrorHandler,  NULL);

	//Registers a callback that will run if an input changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnInputChange_Handler(motorCtrlHandle, MotorInputChangeHandler, NULL);

	//Registers a callback that will run if a motor changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnVelocityChange_Handler(motorCtrlHandle, MotorVelocityChangeHandler, NULL);

	//Registers a callback that will run if the current draw changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnCurrentChange_Handler(motorCtrlHandle, MotorCurrentChangeHandler, NULL);

	//open the motor control for device connections
	CPhidget_open((CPhidgetHandle)motorCtrlHandle, -1);

	//get the program to wait for a motor control device to be attached
	printf("Waiting for MotorControl to be attached....\n");
	#if (!DEBUG_MODE_PC)
	if ((result = CPhidget_waitForAttachment((CPhidgetHandle)motorCtrlHandle, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}
	#endif

	//Display the properties of the attached motor control device
	MotorDisplayProperties(motorCtrlHandle);

	//all done, exit
	return 0;
}
