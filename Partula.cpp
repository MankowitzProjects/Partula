// - MotorControl simple -
// This simple example creates a MotorControl handle, hooks the event handlers, then opens and waits for a MotorControl Phidget to be attached.
// Once on has been attaced it will display device information and display any event data read.  The program will then do a short simulation
// of the functionality of the motor by increasing and decreasing the speed of an attached motor.  PLEASE NOTE:  This assignment
// was desgined assuming only one motor attached at motor index 0 (for motorcontrol boards with multiple motors).
//
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License.
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/
#include &lt;
stdio.h>
#include &lt;
phidget21.h>
#include &lt;
libpowerbutton.h>
&lt;
cr>
//Declare global variables
//Declare a motor control handle
CPhidgetMotorControlHandle motoControl = 0;
//Declare an advanced servo handle
CPhidgetAdvancedServoHandle servo = 0;
int irLeftValue=0;
int irRightValue=0;
int waitTime=1000;
CPhidgetInterfaceKitHandle ifKit = 0;
void go_back(double acc, double spee);
void go_straight(double acc, double spee);
void turn_left(double acc, double spee);
void turn_right(double acc, double spee);
void stop();
int wait(unsigned long int);
double navigSpeed=100.00;
double navigAcc=100.00;
int robotStatus=0;
//**************************************************************************************************
//IR Sensor Initialisation
//**************************************************************************************************
int IRAttachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName(IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);
    printf("%s %10d attached!\n", name, serialNo);
    return 0;
}
int IRDetachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName (IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);
    printf("%s %10d detached!\n", name, serialNo);
    return 0;
}
int IRErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
    printf("Error handled. %d - %s", ErrorCode, unknown);
    return 0;
}
//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int ifKitInputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    if(robotStatus==0)
    {
        navigSpeed=100.00;
    }
    //Hits the left bumper
    if(5==Index && 1==State)
    {
        go_back(navigAcc, navigSpeed);
        turn_right(navigAcc, navigSpeed);
        go_straight(navigAcc, navigSpeed);
    }
    //Hits the right bumper
    if(6==Index && 1==State)
    {
        go_back(navigAcc, navigSpeed);
        turn_left(navigAcc, navigSpeed);
        go_straight(navigAcc, navigSpeed);
    }
    //Hits a Front Whisker
    if(2==Index && 1==State)
    {
        go_back(navigAcc, navigSpeed);
        turn_right(navigAcc, navigSpeed);
        wait(1000);
        go_straight(navigAcc, navigSpeed);
    }
    //Hits a rear Whisker
    if(3==Index && 1==State)
    {
        go_straight(navigAcc, navigSpeed);
        turn_right(navigAcc, navigSpeed);
        wait(1000);
    }
    //The front bumper
    if(4==Index && 1==State)
    {
        stop();
        go_back(navigAcc, navigSpeed);
    }
    printf("Digital Input: %d >
    State: %d\n", Index, State);
    return 0;
}
//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int ifKitOutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
    printf("Digital Output: %d >
    State: %d\n", Index, State);
    return 0;
}
//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int ifKitSensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
    if(6==Index)
    {
        irLeftValue=Value;
    }
    if(7==Index)
    {
        irRightValue=Value;
    }
    if((5==Index) && (Value &lt;
    200))
    {
        robotStatus=1;
        printf("Sensor: %d >
        Value: %d\n", Index, Value);
        navigSpeed=50.00;
        stop();
        go_straight(navigAcc, navigSpeed);
    }
    if(4==Index)
    {
        printf("Sensor: %d >
        Value: %d\n", Index, Value);
    }
    return 0;
}
int display_properties(CPhidgetInterfaceKitHandle phid)
{
    int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
    const char* ptr;
    CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
    CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
    CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
    CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
    CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
    CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
    CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);
    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
    printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
    printf("# Sensors: %d\n", numSensors);
    printf("Ratiometric: %d\n", ratiometric);
    for(i = 0;
    i &lt;
    numSensors;
    i++)
    {
        CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);
        printf("Sensor#: %d >
        Sensitivity Trigger: %d\n", i, triggerVal);
    }
    return 0;
}
void registerIR()
{
    int result, numSensors, i;
    const char *err;
    //create the InterfaceKit object
    CPhidgetInterfaceKit_create(&ifKit);
    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, IRAttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, IRDetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, IRErrorHandler, NULL);
    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, ifKitInputChangeHandler, NULL);
    //Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
    //Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, ifKitSensorChangeHandler, NULL);
    //Registers a callback that will run if an output changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifKit, ifKitOutputChangeHandler, NULL);
    //open the interfacekit for device connections
    CPhidget_open((CPhidgetHandle)ifKit, -1);
    //get the program to wait for an interface kit device to be attached
    printf("Waiting for interface kit to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
    }
    //Display the properties of the attached interface kit device
    display_properties(ifKit);
    printf("Modifying sensor sensitivity triggers....\n");
    //get the number of sensors available
    CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);
    //Change the sensitivity trigger of the sensors
    for(i = 0;
    i &lt;
    numSensors;
    i++)
    {
        CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, 7);
        //we'll just use 10 for fun
    }
    printf("Toggling Ratiometric....\n");
    CPhidgetInterfaceKit_setRatiometric(ifKit, 0);
    //read interface kit event data
    printf("Reading.....\n");
}
void* IR_simple(void *idParam)
{
    // Retrieve data
    int id = *((int*)idParam);
    // Perform some action
    // Count down from 10 using different speed depending on data
    if(id==1)
    {
        //Get the value for IR sensor 1
    }
    else
    {
        //get the value for IR sensor 2
    }
    // Exit thread
    pthread_exit(NULL);
}
void unregisterIR()
{
    //since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
    printf("Closing...\n");
    CPhidget_close((CPhidgetHandle)ifKit);
    CPhidget_delete((CPhidgetHandle)ifKit);
}
//**************************************************************************************************
//Thread Functions
// Consistent wait function (use instead of other implementations)
int wait(unsigned long milisec)
{
    struct timespec req=
    {
        0
    }
    ;
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
    continue;
    return 1;
}
//*******************************************************
//************************************************
//Servo Code
//***********************************************
int ServoAttachHandler(CPhidgetHandle ADVSERVO, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName (ADVSERVO, &name);
    CPhidget_getSerialNumber(ADVSERVO, &serialNo);
    printf("%s %10d attached!\n", name, serialNo);
    return 0;
}
int ServoDetachHandler(CPhidgetHandle ADVSERVO, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName (ADVSERVO, &name);
    CPhidget_getSerialNumber(ADVSERVO, &serialNo);
    printf("%s %10d detached!\n", name, serialNo);
    return 0;
}
int ServoErrorHandler(CPhidgetHandle ADVSERVO, void *userptr, int ErrorCode, const char *Description)
{
    printf("Error handled. %d - %s\n", ErrorCode, Description);
    return 0;
}
int PositionChangeHandler(CPhidgetAdvancedServoHandle ADVSERVO, void *usrptr, int Index, double Value)
{
    //    printf("Motor: %d >
    Current Position: %f\n", Index, Value);
    return 0;
}
//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
int display_properties(CPhidgetAdvancedServoHandle phid)
{
    int serialNo, version, numMotors;
    const char* ptr;
    CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
    CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
    CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
    CPhidgetAdvancedServo_getMotorCount (phid, &numMotors);
    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n# Motors: %d\n", serialNo, version, numMotors);
    return 0;
}
int registerServo()
{
    int result;
    double curr_pos;
    const char *err;
    double minAccel, maxVel;
    //create the advanced servo object
    CPhidgetAdvancedServo_create(&servo);
    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)servo, ServoAttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, ServoDetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)servo, ServoErrorHandler, NULL);
    //Registers a callback that will run when the motor position is changed.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetAdvancedServo_set_OnPositionChange_Handler(servo, PositionChangeHandler, NULL);
    //open the device for connections
    CPhidget_open((CPhidgetHandle)servo, -1);
    //get the program to wait for an advanced servo device to be attached
    printf("Waiting for Phidget to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)servo, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
        return 0;
    }
    //Display the properties of the attached device
    display_properties(servo);
    //read event data
    printf("Reading.....\n");
    //This example assumes servo motor is attached to index 0
    //Set up some initial acceleration and velocity values
    //    CPhidgetAdvancedServo_getAccelerationMin(servo, 0, &minAccel);
    //    CPhidgetAdvancedServo_setAcceleration(servo, 0, minAccel*2);
    //    CPhidgetAdvancedServo_getVelocityMax(servo, 0, &maxVel);
    //    CPhidgetAdvancedServo_setVelocityLimit(servo, 0, maxVel/2);
    //display current motor position
    if(CPhidgetAdvancedServo_getPosition(servo, 0, &curr_pos) == EPHIDGET_OK)
    printf("Servo Motor: 0 >
    Current Position: %f\n", curr_pos);
}
void unregisterServo()
{
    //since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
    printf("Closing...\n");
    CPhidget_close((CPhidgetHandle)servo);
    CPhidget_delete((CPhidgetHandle)servo);
}
//Move the servo
void moveServo(double position)
{
    CPhidgetAdvancedServo_setPosition (servo, 0, position);
}
//*****************************************************************************
int AttachHandler(CPhidgetHandle MC, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName (MC, &name);
    CPhidget_getSerialNumber(MC, &serialNo);
    printf("%s %10d attached!\n", name, serialNo);
    return 0;
}
int DetachHandler(CPhidgetHandle MC, void *userptr)
{
    int serialNo;
    const char *name;
    CPhidget_getDeviceName (MC, &name);
    CPhidget_getSerialNumber(MC, &serialNo);
    printf("%s %10d detached!\n", name, serialNo);
    return 0;
}
int ErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description)
{
    printf("Error handled. %d - %s\n", ErrorCode, Description);
    return 0;
}
int InputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State)
{
    printf("Input %d >
    State: %d\n", Index, State);
    return 0;
}
int VelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
    //    printf("Motor %d >
    Current Speed: %f\n", Index, Value);
    return 0;
}
int CurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
    printf("Motor: %d >
    Current Draw: %f\n", Index, Value);
    return 0;
}
int display_properties(CPhidgetMotorControlHandle phid)
{
    int serialNo, version, numInputs, numMotors;
    const char* ptr;
    CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
    CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
    CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
    CPhidgetMotorControl_getInputCount(phid, &numInputs);
    CPhidgetMotorControl_getMotorCount(phid, &numMotors);
    printf("%s\n", ptr);
    printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
    printf("# Inputs: %d\n# Motors: %d\n", numInputs, numMotors);
    return 0;
}
void go_straight(double acc, double speed)
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0, -acc);
    CPhidgetMotorControl_setVelocity (motoControl, 0, -speed);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, acc);
    CPhidgetMotorControl_setVelocity (motoControl, 1, speed);
    wait(waitTime);
}
void go_back(double acc, double speed)
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0, acc);
    CPhidgetMotorControl_setVelocity (motoControl, 0, speed);
    CPhidgetMotorControl_setAcceleration (motoControl, 1,  -acc);
    CPhidgetMotorControl_setVelocity (motoControl, 1,  -speed);
    wait(waitTime);
}
void turn_left(double acc, double speed)
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0,  -acc);
    CPhidgetMotorControl_setVelocity (motoControl, 0,  -speed);
    CPhidgetMotorControl_setAcceleration (motoControl, 1,  -acc);
    CPhidgetMotorControl_setVelocity (motoControl, 1,  -speed);
    wait(waitTime);
}
void turn_right(double acc, double speed)
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0, acc);
    CPhidgetMotorControl_setVelocity (motoControl, 0, speed);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, acc);
    CPhidgetMotorControl_setVelocity (motoControl, 1, speed);
    wait(waitTime);
}
void stop()
{
    CPhidgetMotorControl_setAcceleration (motoControl, 0, 0.00);
    CPhidgetMotorControl_setVelocity (motoControl, 0, 0.00);
    CPhidgetMotorControl_setAcceleration (motoControl, 1, 0.00);
    CPhidgetMotorControl_setVelocity (motoControl, 1, 0.00);
    wait(waitTime);
}
int register_motors()
{
    int result;
    const char *err;
    //create the motor control object
    CPhidgetMotorControl_create(&motoControl);
    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)motoControl, AttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)motoControl, DetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)motoControl, ErrorHandler, NULL);
    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnInputChange_Handler (motoControl, InputChangeHandler, NULL);
    //Registers a callback that will run if a motor changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnVelocityChange_Handler (motoControl, VelocityChangeHandler, NULL);
    //Registers a callback that will run if the current draw changes.
    //Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetMotorControl_set_OnCurrentChange_Handler (motoControl, CurrentChangeHandler, NULL);
    //open the motor control for device connections
    CPhidget_open((CPhidgetHandle)motoControl, -1);
    //get the program to wait for a motor control device to be attached
    printf("Waiting for MotorControl to be attached....");
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)motoControl, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
        return 0;
    }
    //Display the properties of the attached motor control device
    display_properties(motoControl);
    //read motor control event data
    printf("Reading.....\n");
    //keep displaying motor control event data until user input is read
    printf("Press any key to continue\n");
    getchar();
}
void unregister_motors()
{
    //since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
    printf("Closing...\n");
    CPhidget_close((CPhidgetHandle)motoControl);
    CPhidget_delete((CPhidgetHandle)motoControl);
}
int main(int argc, char* argv[])
{
    /*printf("Button pressed %i times.\n",power_button_get_value());
    */
    register_motors();
    //Regsiter the IR sensors
    registerIR();
    //Register the servo
    registerServo();
    //sleep(1);
    /*printf("Button pressed %i times.\n",power_button_get_value());
    */
    //Motor Status:0=stop;
    1=forward;
    2=backward;
    3=left;
    4=right;
    int motorStatus=1;
    int irLeft=1;
    int irRight=2;
    //Run IR sensors in a separate thread
    //Create thread 1 for IR sensor Left
    pthread_t irThread;
    pthread_create(&irThread, NULL, IR_simple, (void*) &irLeft );
    //Create thread 2 for IR sensor Right
    pthread_t irThread2;
    pthread_create(&irThread2, NULL, IR_simple, (void*) &irRight );
    go_straight(navigAcc, navigSpeed);
    while(power_button_get_value()&lt;
    1)
    {
        while(robotStatus==0)
        {
            //Create the motor movement routine
            if(irLeftValue>
            300 && motorStatus!=4)
            {
                printf("Turning right");
                turn_right(navigAcc, navigSpeed);
                motorStatus=4;
                //wait(500);
            }
            if(irRightValue>
            300 && motorStatus!=3)
            {
                printf("Turning left");
                turn_left(navigAcc, navigSpeed);
                motorStatus=3;
                //wait(500);
            }
            if(irLeftValue && irRightValue >
            300 && motorStatus!=2)
            {
                printf("Moving back");
                stop();
                //wait(1000);
                go_back(navigAcc, navigSpeed);
                motorStatus=2;
                //wait(500);
            }
            else if(motorStatus!=1)
            {
                motorStatus=1;
                printf("Moving forward");
                go_straight(navigAcc, navigSpeed);
                //wait(500);
            }
        }
        //End navigating while
        //If the robot wants to scan the area
        while (robotStatus==1)
        {
            // stop();
            // moveServo(100.00);
            // wait(1000);
            // moveServo(180.00);
            // wait(1000);
        }
    }
    //end main while
    unregisterIR();
    unregister_motors();
    unregisterServo();
    power_button_reset();
    return 0;
}
