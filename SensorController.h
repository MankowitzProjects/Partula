#ifndef SENSORCONTROLLER_H_INCLUDED
#define SENSORCONTROLLER_H_INCLUDED

#include "conf.h"

#include "Sensor.h"
#include "Switch.h"

using namespace std;

class SensorController
{
public:
    SensorController(void);

    void init(void);
    void fin(void);

    CPhidgetInterfaceKitHandle sensorCtrlHandle;

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

    inline void         setSensorValue(const int index, const int value);
    inline int          getSensorValue(const int index);
    inline int          getSensorValuePre(const int index);
    inline int          getSensorValueAvrg(const int index);

    inline void         setSensorType(const int index, const TYPE_SENSOR type);
    inline TYPE_SENSOR  getSensorType(const int index);

    inline POSITION     getSensorPos(const int index);

    inline void         setSwitchState(const int index, const int state);
    inline int          getSwitchState(const int index);

    inline void         setSwitchType(const int index, const TYPE_SWITCH type);
    inline TYPE_SWITCH  getSwitchType(const int index);

    inline POSITION     getSwitchPos(const int index);
};


/** \brief Set the value for a sensor regarding to it's index number
 *
 * \param index const int
 * \param value const int
 * \return void
 *
 */
inline void SensorController::setSensorValue(const int index, const int value)
{
    #if (DEBUG_MODE_SENSOR_CONTROLLER)
    printf("SensorController::setSensorValue: index = %d, type: %s, position: %s value: %d\n",
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

inline int SensorController::getSensorValueAvrg(const int index)
{
    return sensorTable[index].getValueAvrg();
}

inline int SensorController::getSensorValuePre(const int index)
{
    return sensorTable[index].getValuePre();
}

inline void SensorController::setSensorType(const int index, const TYPE_SENSOR type)
{
    sensorTable[index].setType(type);
}

inline TYPE_SENSOR SensorController::getSensorType(const int index)
{
    return sensorTable[index].getType();
}

inline POSITION SensorController::getSensorPos(const int index)
{
    return sensorTable[index].getPos();
}

inline POSITION SensorController::getSwitchPos(const int index)
{
    return switchTable[index].getPos();
}

/** \brief Set the value for a switch regarding to it's index number
 *
 * \param index const int
 * \param value const int
 * \return void
 *
 */
inline void SensorController::setSwitchState(const int index, const int state)
{
    #if (DEBUG_MODE_SENSOR_CONTROLLER)
    printf("SensorController::setSwitchValue: index = %d, type: %s, position: %s state: %d\n",
           index, GetSwitchTypeChar(switchTable[index].getType()), GetPositionChar(switchTable[index].getPos()), state);
    #endif

    switchTable[index].setValue(state);
    lstRptSwitchID = index;
}

/** \brief Get the switch value by given switch index
 *
 * \param index const int   The switch index
 * \return int
 *
 */
inline int SensorController::getSwitchState(const int index)
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
//Need to change these values based on training data
#define VALUE_BLACK_PAPER_MIN   480
#define VALUE_BLACK_PAPER_MAX   580

#define VALUE_BLACK_TAPE_MIN    90
#define VALUE_BLACK_TAPE_MAX    150

#define VALUE_GROUND_MIN        200
#define VALUE_GROUND_MAX        260

inline bool bIsInRange(int value, int valueMin, int valueMax)
{
    return ((valueMin < value) && (value < valueMax));
}

inline bool bIsBlackPaper(int value)
{
    return bIsInRange(value, VALUE_BLACK_TAPE_MIN, VALUE_BLACK_TAPE_MAX);
}

inline bool bIsBlackTape(int value)
{
    return bIsInRange(value, VALUE_BLACK_PAPER_MIN, VALUE_BLACK_PAPER_MAX);
}

inline bool bIsGround(int value)
{
    return bIsInRange(value, VALUE_GROUND_MIN, VALUE_GROUND_MAX);
}

TYPE_SWITCH GetSwitchType(int index);
POSITION    GetSwitchPos(int index);
int         GetSwitchState(int index);

TYPE_SENSOR GetSensorType(int index);
POSITION    GetSensorPos(int index);
int         GetSensorValue(int index);
int         GetSensorValueAvrg(int index);
int         GetSensorValuePre(int index);

#endif // SENSORCONTROLLER_H_INCLUDED
