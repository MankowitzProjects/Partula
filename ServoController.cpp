#include "conf.h"
#include "ServoController.h"

ServoController g_servoCtrl;

void ServoController::init(void)
{
    curPos = 0.0;
    regHandlers();

    CPhidgetAdvancedServo_setEngaged(srvCtrlHandle, 0, 1);
}

void ServoController::fin(void)
{
    CPhidgetAdvancedServo_setEngaged(srvCtrlHandle, 0, 0);

    clrHandlers();
}

double ServoController::getPos(void)
{
    CPhidgetAdvancedServo_getPosition(srvCtrlHandle, 0, &curPos);
    return curPos;
}

void ServoController::setPos(double pos)
{
    #if (DEBUG_MODE_SERVO_CONTROLLER)
    cout << "ServoController::setPos - " << pos << endl;
    #endif
    CPhidgetAdvancedServo_setPosition(srvCtrlHandle, 0, pos);
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ServoAttachHandler(CPhidgetHandle SERV, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (SERV, &name);
	CPhidget_getSerialNumber(SERV, &serialNo);
	printf("Servo %s %10d attached!\n", name, serialNo);

	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ServoDetachHandler(CPhidgetHandle SERV, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (SERV, &name);
	CPhidget_getSerialNumber(SERV, &serialNo);
	printf("Servo %s %10d detached!\n", name, serialNo);

	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ServoErrorHandler(CPhidgetHandle SERV, void *userptr, int ErrorCode, const char *Description)
{
	printf("Servo error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ServoPositionChangeHandler(CPhidgetAdvancedServoHandle SERV, void *usrptr, int Index, double Value)
{
    #if DEBUG_MODE_SERVO_CONTROLLER
	printf("Servo Motor: %d > Current Position: %f\n", Index, Value);
	#endif
	return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
int servo_display_properties(CPhidgetAdvancedServoHandle phid)
{
	int serialNo, version, numMotors;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetAdvancedServo_getMotorCount(phid, &numMotors);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n# Motors: %d\n", serialNo, version, numMotors);

	return 0;
}

void ServoController::regHandlers(void)
{
	int result = 0;
	const char *err = NULL;

	//create the servo object
	CPhidgetAdvancedServo_create(&srvCtrlHandle);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)srvCtrlHandle, ServoAttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)srvCtrlHandle, ServoDetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)srvCtrlHandle, ServoErrorHandler, NULL);

	//Registers a callback that will run when the motor position is changed.
	//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetAdvancedServo_set_OnPositionChange_Handler(srvCtrlHandle, ServoPositionChangeHandler, NULL);

	//open the servo for device connections
	CPhidget_open((CPhidgetHandle)srvCtrlHandle, -1);

    #if (0 == DEBUG_MODE_PC)
	//get the program to wait for an servo device to be attached
	printf("Waiting for Servo controller to be attached....\n");
	if ((result = CPhidget_waitForAttachment((CPhidgetHandle)srvCtrlHandle, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
	}
	#endif

	//Display the properties of the attached servo device
	servo_display_properties(srvCtrlHandle);
}

void ServoController::clrHandlers(void)
{
	CPhidget_close((CPhidgetHandle)srvCtrlHandle);
	CPhidget_delete((CPhidgetHandle)srvCtrlHandle);
}
