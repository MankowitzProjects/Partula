#ifndef SENSORCONTROLLER_H_INCLUDED
#define SENSORCONTROLLER_H_INCLUDED

#include "Sensor.h"
#include "Switch.h"

class SensorController
{
public:
    SensorController(void);

    void init(void);
    void fin(void);

    CPhidgetInterfaceKitHandle ifkControl;

    /**< Stores all the sensor information, the init() function
         will be called when the table is created */
    Sensor sensorTable[NUM_SENSOR_SLOTS];
    Switch switchTable[NUM_SWITCH_SLOTS];

    void regHandlers(void);
    void clrHandlers(void);

    int lstRptSensorID;     /**< The last reported sensor index */
    int lstRptSwitchID;     /**< The last reported switch index */


    void setupSensors(void);
    void setupSwitches(void);

    inline void setSensorValue(const int index, const int value);
    inline int  getSensorValue(const int index);

    inline void setSwitchValue(const int index, const int value);
    inline int  getSwitchValue(const int index);

    inline void        setSensorType(const int index, const TYPE_SENSOR type);
    inline TYPE_SENSOR getSensorType(const int index);

    inline void        setSwitchType(const int index, const TYPE_SWITCH type);
    inline TYPE_SWITCH getSwitchType(const int index);
};

extern SensorController g_sensoCtrl;

/** \brief Default constructor
 *SensorController g_sensoCtrl;

 * \param void
 *
 */
SensorController::SensorController(void)
{
    ;
}

void SensorController::setupSwitches(void)
{
    // Bumper switch LEFT
    switchTable[INDEX_SWITCH_BUMPER_LEFT].init(INDEX_SWITCH_BUMPER_LEFT, TYPE_SWITCH_BUMPER, POSITION_LEFT);
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
    // IR sensor LEFT
    sensorTable[INDEX_SENSOR_IR_LEFT].init (INDEX_SENSOR_IR_LEFT, TYPE_SENSOR_IR, POSITION_LEFT);
    // IR sensor RIGHT
    sensorTable[INDEX_SENSOR_IR_RIGHT].init(INDEX_SENSOR_IR_RIGHT, TYPE_SENSOR_IR, POSITION_RIGHT);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_LEFT].init (INDEX_SENSOR_LIGHT_LEFT, TYPE_SENSOR_LIGHT, POSITION_LEFT);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_MIDDLE].init (INDEX_SENSOR_LIGHT_MIDDLE, TYPE_SENSOR_LIGHT, POSITION_MIDDLE);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_RIGHT].init (INDEX_SENSOR_LIGHT_RIGHT, TYPE_SENSOR_LIGHT, POSITION_RIGHT);
    // Light sensor UNDER
    sensorTable[INDEX_SENSOR_LIGHT_UNDER].init (INDEX_SENSOR_LIGHT_UNDER, TYPE_SENSOR_LIGHT, POSITION_UNDER);
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

    prt_debug("SensorController::init done!\n");
}

/** \brief Finalize the class
 *
 * \param void
 * \return void
 *
 */
void SensorController::fin(void)
{
    clrHandlers();
}

/** \brief Set the value for a sensor regarding to it's index number
 *
 * \param index const int
 * \param value const int
 * \return void
 *
 */
inline void SensorController::setSensorValue(const int index, const int value)
{
    #if (DEBUG_MODE)
    printf("SensorController::setSensorValue: index = %d, type: %8s, position: %8s value: %10d\n",
           index, GetSensorTypeChar(sensorTable[index].getType()), GetPositionChar(sensorTable[index].getPos()), value);
    #endif

    sensorTable[index].setValue(value);
    lstRptSensorID = index;
}

/** \brief Get the sensor value by given sensor index
 *
 * \param index const int   The sensor index
 * \return int
 *
 */
inline int SensorController::getSensorValue(const int index)
{
    return sensorTable[index].getValue();
}

inline void SensorController::setSensorType(const int index, const TYPE_SENSOR type)
{
    sensorTable[index].setType(type);
}

inline TYPE_SENSOR SensorController::getSensorType(const int index)
{
    return sensorTable[index].getType();
}

/** \brief Set the value for a switch regarding to it's index number
 *
 * \param index const int
 * \param value const int
 * \return void
 *
 */
inline void SensorController::setSwitchValue(const int index, const int value)
{
    #if (DEBUG_MODE)
    printf("SensorController::setSwitchValue: index = %d, type: %8s, position: %8s value: %10d\n",
           index, GetSwitchTypeChar(switchTable[index].getType()), GetPositionChar(switchTable[index].getPos()), value);
    #endif

    switchTable[index].setValue(value);
    lstRptSwitchID = index;
}

/** \brief Get the switch value by given switch index
 *
 * \param index const int   The switch index
 * \return int
 *
 */
inline int SensorController::getSwitchValue(const int index)
{
    return switchTable[index].getValue();
}

inline void SensorController::setSwitchType(const int index, const TYPE_SWITCH type)
{
    switchTable[index].setType(type);
}

inline TYPE_SWITCH SensorController::getSwitchType(const int index)
{
    return switchTable[index].getType();
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
    printf("Error handled. %d - %s", ErrorCode, unknown);
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
#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
__stdcall
#endif
ifKitInputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int index, int state)
{
    g_sensoCtrl.setSwitchValue(index, state);

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
ifKitSensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int index, int state)
{
    g_sensoCtrl.setSensorValue(index, state);

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

    for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);
        printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
    }

    return 0;
}

void SensorController::regHandlers(void)
{
    int result, numSensors, i;
    const char *err;

    //create the InterfaceKit object
    CPhidgetInterfaceKit_create(&ifkControl);

    //Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifkControl, ifKitAttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifkControl, ifKitDetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)ifkControl, ifKitErrorHandler, NULL);

    //Registers a callback that will run if an input changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnInputChange_Handler (ifkControl, ifKitInputChangeHandler, NULL);

    //Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
    //Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifkControl, ifKitSensorChangeHandler, NULL);

    //Registers a callback that will run if an output changes.
    //Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
    CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifkControl, ifKitOutputChangeHandler, NULL);

    //open the interfacekit for device connections
    CPhidget_open((CPhidgetHandle)ifkControl, -1);

    //get the program to wait for an interface kit device to be attached
    printf("Waiting for interface kit to be attached....");

    if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifkControl, 10000)))
    {
        CPhidget_getErrorDescription(result, &err);
        printf("Problem waiting for attachment: %s\n", err);
    }

    //Display the properties of the attached interface kit device
    display_properties(ifkControl);

    printf("Modifying sensor sensitivity triggers....\n");

    //get the number of sensors available
    CPhidgetInterfaceKit_getSensorCount(ifkControl, &numSensors);

    //Change the sensitivity trigger of the sensors
    for(i = 0; i < numSensors; i++)
    {
        CPhidgetInterfaceKit_setSensorChangeTrigger(ifkControl, i, 20);  //we'll just use 10 for fun
    }

    printf("Toggling Ratiometric....\n");
    CPhidgetInterfaceKit_setRatiometric(ifkControl, 0);

    //read interface kit event data
    printf("Reading.....\n");
}

void SensorController::clrHandlers(void)
{
    printf("SensorController::clrHandlers - Closing...\n");

    CPhidget_close((CPhidgetHandle)ifkControl);
    CPhidget_delete((CPhidgetHandle)ifkControl);

    printf("SensorController::clrHandlers - Closed.\n");
}

#endif // SENSORCONTROLLER_H_INCLUDED
