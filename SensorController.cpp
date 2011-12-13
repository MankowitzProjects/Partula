#include "SensorController.h"
#include "Event.h"

SensorController g_sensorCtrl;

extern Event g_eventCenter;
int VALUE_BLACK_PAPER_MIN  = 480;
int VALUE_BLACK_PAPER_MAX  = 580;

int VALUE_BLACK_TAPE_MIN  =  60;
int VALUE_BLACK_TAPE_MAX  =  120;

int VALUE_GROUND_MIN    = 200;
int VALUE_GROUND_MAX     =   260;

/** \brief Default constructor
 * \param void
 *
 */
SensorController::SensorController(void)
{
    ;
}

/** \brief Initialize the sensor controller
 *
 * \param void
 * \return void
 *
 */
void SensorController::init(void)
{
    prt_debug("SensorController::init\n");

    regHandlers();

    setupSwitches();
    setupSensors();

    prt_debug("SensorController::init - DONE~!\n");
}

/** \brief Finalize the class
 *
 * \param void
 * \return void
 *
 */
void SensorController::fin(void)
{
    printf("SensorController::fin\n");
    clrHandlers();
    printf("SensorController::fin - DONE~!\n");
}

void SensorController::setupSwitches(void)
{
    // Bumper switch LEFT
    switchTable[INDEX_SWITCH_BUMPER_LEFT ].init(INDEX_SWITCH_BUMPER_LEFT,  TYPE_SWITCH_BUMPER, POSITION_LEFT);
    // Bumper switch RIGHT
    switchTable[INDEX_SWITCH_BUMPER_RIGHT].init(INDEX_SWITCH_BUMPER_RIGHT, TYPE_SWITCH_BUMPER, POSITION_RIGHT);
    // Bumper switch LEFT
    switchTable[INDEX_SWITCH_BUMPER_FRONT].init(INDEX_SWITCH_BUMPER_FRONT, TYPE_SWITCH_BUMPER, POSITION_FRONT);
}

/** \brief Set up the information of the sensors installed
 *
 * \param void
 * \return void
 *
 */
void SensorController::setupSensors(void)
{
    // IR sensor TOP
    sensorTable[INDEX_SENSOR_IR_TOP      ].init(INDEX_SENSOR_IR_TOP,       TYPE_SENSOR_IR,    POSITION_MIDDLE);
    // IR sensor BOTTOM
    sensorTable[INDEX_SENSOR_IR_BOTTOM   ].init(INDEX_SENSOR_IR_BOTTOM,    TYPE_SENSOR_IR,    POSITION_UNDER);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_LEFT  ].init(INDEX_SENSOR_LIGHT_LEFT,   TYPE_SENSOR_LIGHT, POSITION_LEFT);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_MIDDLE].init(INDEX_SENSOR_LIGHT_MIDDLE, TYPE_SENSOR_LIGHT, POSITION_MIDDLE);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_RIGHT ].init(INDEX_SENSOR_LIGHT_RIGHT,  TYPE_SENSOR_LIGHT, POSITION_RIGHT);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_UNDER ].init(INDEX_SENSOR_LIGHT_UNDER,  TYPE_SENSOR_LIGHT, POSITION_UNDER);
    // Sonar sensor
    sensorTable[INDEX_SENSOR_SONAR       ].init(INDEX_SENSOR_SONAR,        TYPE_SENSOR_SONAR, POSITION_FRONT);
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ifKitAttachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName(IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);

    printf("%s %10d attached!\n", name, serialNo);

    return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ifKitDetachHandler(CPhidgetHandle IFK, void *userptr)
{
    int serialNo;
    const char *name;

    CPhidget_getDeviceName (IFK, &name);
    CPhidget_getSerialNumber(IFK, &serialNo);

    printf("%s %10d detached!\n", name, serialNo);

    return 0;
}

int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ifKitErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
    printf("** ERROR: ifkit Error handled. %d - %s\n", ErrorCode, unknown);
    return 0;
}

/** \brief The bumpers will report to this handler
 *         callback that will run if an input changes.
 *
 *
 * \param IFK CPhidgetInterfaceKitHandle
 * \param usrptr void*
 * \param Index int Index of the input that generated the event
 * \param State int boolean (0 or 1) representing the input state (on or off)
 * \return int
 *
 */
int
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
__stdcall
#endif
ifKitInputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int index, int state)
{
    if (g_sensorCtrl.switchTable[index].isExist())
    {
        INPUT switchInput;
        switchInput.type    = TYPE_INPUT_SWITCH;
        switchInput.subType = g_sensorCtrl.getSwitchType(index);
        switchInput.index   = index;
        switchInput.value   = state;
        switchInput.pos     = g_sensorCtrl.getSwitchPos(index);

        g_sensorCtrl.setSwitchState(index, state);

        #if (!DEBUG_MODE_BLOCK_SENSORS)
        g_eventCenter.handleInput(switchInput);
        #endif
    }

    return 0;
}

//callback that will run if an output changes.

//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ifKitOutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int index, int state)
{
    #if (DEBUG_MODE_LOW)
    printf("Digital Output: %d > State: %d\n", index, state);
    #endif
    return 0;
}

/** \brief callback that will run if the sensor value changes by more than the OnSensorChange trigger.
 *         IR and Light sensors~
 *
 * \param IFK CPhidgetInterfaceKitHandle
 * \param usrptr void*
 * \param Index int Index of the sensor that generated the event
 * \param Value int the sensor read value
 * \return int
 *
 */
int
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
__stdcall
#endif
ifKitSensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int index, int value)
{
    //Sets the sensor value based on its index
    //The sensor values can be IR, Light Sensors
    if (g_sensorCtrl.sensorTable[index].isExist())
    {
        INPUT sensorInput;
        sensorInput.type    = TYPE_INPUT_SENSOR;
        sensorInput.subType = g_sensorCtrl.getSensorType(index);
        sensorInput.index   = index;
        sensorInput.value   = value;
        sensorInput.pos     = g_sensorCtrl.getSensorPos(index);

        g_sensorCtrl.setSensorValue(index, value);

        #if (!DEBUG_MODE_BLOCK_SENSORS)
        g_eventCenter.handleInput(sensorInput);
        #endif
    }

    return 0;
}

int display_properties(CPhidgetInterfaceKitHandle phid)
{
    int serialNo, version, numInputs = 0, numOutputs = 0, numSensors = 0, triggerVal, ratiometric, i;
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

    for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);
        printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
    }

    return 0;
}

void SensorController::regHandlers(void)
{
    int result = 0;
    int numSensors = 0;
    #if (!DEBUG_MODE_PC)
    const char *err;
    #endif

    //create the InterfaceKit object
    CPhidgetInterfaceKit_create(&sensorCtrlHandle);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)sensorCtrlHandle, ifKitAttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)sensorCtrlHandle, ifKitDetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)sensorCtrlHandle, ifKitErrorHandler, NULL);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnInputChange_Handler(sensorCtrlHandle, ifKitInputChangeHandler, NULL);

    //Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
    //Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnSensorChange_Handler(sensorCtrlHandle, ifKitSensorChangeHandler, NULL);

    //Registers a callback that will run if an output changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnOutputChange_Handler(sensorCtrlHandle, ifKitOutputChangeHandler, NULL);

    //open the interfacekit for device connections
    CPhidget_open((CPhidgetHandle)sensorCtrlHandle, -1);

    //get the program to wait for an interface kit device to be attached
    printf("Waiting for interface kit to be attached....\n");
	#if (!DEBUG_MODE_PC)
    if((result = CPhidget_waitForAttachment((CPhidgetHandle)sensorCtrlHandle, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
    }
    #endif

    //Display the properties of the attached interface kit device
    display_properties(sensorCtrlHandle);

    printf("Modifying sensor sensitivity triggers....\n");

    //get the number of sensors available
    CPhidgetInterfaceKit_getSensorCount(sensorCtrlHandle, &numSensors);

    //Change the sensitivity trigger of the sensors
    for (int i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_setSensorChangeTrigger(sensorCtrlHandle, i, 6);  //we'll just use 10 for fun
    }

    //CPhidgetInterfaceKit_setSensorChangeTrigger(sensorCtrlHandle, INDEX_SENSOR_IR_TOP, 4);
    //CPhidgetInterfaceKit_setSensorChangeTrigger(sensorCtrlHandle, INDEX_SENSOR_IR_BOTTOM, 4);


    CPhidgetInterfaceKit_setSensorChangeTrigger(sensorCtrlHandle, INDEX_SENSOR_SONAR, 5);

    printf("Toggling Ratiometric....\n");
    CPhidgetInterfaceKit_setRatiometric(sensorCtrlHandle, 0);

    //read interface kit event data
    printf("Reading.....\n");
}

void SensorController::clrHandlers(void)
{
    printf("SensorController::clrHandlers...\n");

    CPhidget_close( (CPhidgetHandle)sensorCtrlHandle);
    CPhidget_delete((CPhidgetHandle)sensorCtrlHandle);

    printf("SensorController::clrHandlers - DONE~!\n");
}

TYPE_SWITCH GetSwitchType(int index)
{
    return g_sensorCtrl.getSwitchType(index);
}

POSITION GetSwitchPos(int index)
{
    return g_sensorCtrl.getSwitchPos(index);
}

int GetSwitchState(int index)
{
    return g_sensorCtrl.getSwitchState(index);
}

TYPE_SENSOR GetSensorType(int index)
{
    return g_sensorCtrl.getSensorType(index);
}

POSITION GetSensorPos(int index)
{
    return g_sensorCtrl.getSensorPos(index);
}

int GetSensorValue(int index)
{
    return g_sensorCtrl.getSensorValue(index);
}

int GetSensorValuePre(int index)
{
    return g_sensorCtrl.getSensorValuePre(index);
}

int GetSensorValueAvrg(int index)
{
    return g_sensorCtrl.getSensorValueAvrg(index);
}
