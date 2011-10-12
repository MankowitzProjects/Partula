#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED

#include <stdio.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
    #include "./include/phidget21.h"
#else
   #include <phidget21.h>
#endif

#include "debug_tools.h"

//*********************************** NULL ***********************************
#ifndef NULL
#define NULL 0
#endif

#define VALUE_NULL      NULL
#define STATE_NULL      NULL
#define POINTER_NULL    NULL


#define STATE_ON    1
#define STATE_OFF   0

//*********************************** LEGO ***********************************
#define NUM_SENSOR_SLOTS 8
#define NUM_SWITCH_SLOTS 8

//*********************************** INDEX **********************************
// Sensors
#define INDEX_SENSOR_IR_LEFT        7
#define INDEX_SENSOR_IR_RIGHT       6

#define INDEX_SENSOR_LIGHT_LEFT     4
#define INDEX_SENSOR_LIGHT_MIDDLE   3
#define INDEX_SENSOR_LIGHT_RIGHT    2
#define INDEX_SENSOR_LIGHT_UNDER    5

// Switches
#define INDEX_SWITCH_BUMPER_LEFT    5
#define INDEX_SWITCH_BUMPER_FRONT   4
#define INDEX_SWITCH_BUMPER_RIGHT   6

//*********************************** TYPE ***********************************
const char *g_sensorTypeDis[] = {
/* 0x00 */  "null",
/* 0x01 */  "IR",
/* 0x02 */  "light",
/* 0x03 */  "bumper",
/* 0x04 */  "whisker"
};

typedef enum
{
    TYPE_SENSOR_NULL = 0x00,
    TYPE_SENSOR_IR,
    TYPE_SENSOR_LIGHT

} TYPE_SENSOR;


static inline const char *GetSensorTypeChar(TYPE_SENSOR type)
{
    return g_sensorTypeDis[type];
}

//*********************************** TYPE ***********************************
const char *g_swtichTypeDis[] = {
/* 0x00 */  "null",
/* 0x01 */  "bumper",
/* 0x02 */  "whisker"
};

typedef enum
{
    TYPE_SWITCH_NULL = 0x00,
    TYPE_SWITCH_BUMPER,
    TYPE_SWITCH_WHISKER

} TYPE_SWITCH;

static inline const char *GetSwitchTypeChar(TYPE_SWITCH type)
{
    return g_swtichTypeDis[type];
}

//********************************* POSITION *********************************
const char *g_position[] =
{
/* 0x00 */  "null",
/* 0x01 */  "front",
/* 0x02 */  "back",
/* 0x03 */  "left",
/* 0x04 */  "right",
/* 0x05 */  "middle",
/* 0x06 */  "under"
};

typedef enum
{
    POSITION_NULL   = 0x00,
    POSITION_FRONT  = 0x01,
    POSITOIN_BACK   = 0x02,
    POSITION_LEFT   = 0x03,
    POSITION_RIGHT  = 0x04,
    POSITION_MIDDLE = 0x05,
    POSITION_UNDER  = 0x06

} POSITION;

static inline const char *GetPositionChar(POSITION position)
{
     return g_position[position];
}
#define INDEX_MOTOR_LEFT    0
#define INDEX_MOTOR_RIGHT   1

#endif // CONF_H_INCLUDED
