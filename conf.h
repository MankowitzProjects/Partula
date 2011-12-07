#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED

#include <stdio.h>
#include <iostream>

#if defined(_MSC_VER) && (_MSC_VER >= 1200 )
    #include "./include/phidget21.h"
    #include "./include/pthread.h"
#else
   #include <phidget21.h>
   #include <pthread.h>
#endif

#include "debug_tools.h"

using namespace std;

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

#define VALUE_MOTOR_VEL_NORMAL  100
#define VALUE_MOTOR_ACC_NORMAL  100

#define VALUE_MIN_LED_LIGHT     300

#define VALUE_SERVO_POS_MIN     0.0
#define VALUE_SERVO_POS_MAX   220.0
#define VALUE_SERVO_POS_MID   110.0

//*********************************** INDEX **********************************
// Sensors
//#define INDEX_SENSOR_IR_LEFT        7

#define INDEX_SENSOR_SONAR          1

#define INDEX_SENSOR_LIGHT_RIGHT    2
#define INDEX_SENSOR_LIGHT_MIDDLE   3
#define INDEX_SENSOR_LIGHT_LEFT     4
#define INDEX_SENSOR_LIGHT_UNDER    5

#define INDEX_SENSOR_IR_BOTTOM      6
#define INDEX_SENSOR_IR_TOP         7

// Switches
#define INDEX_SWITCH_BUMPER_LEFT    4
#define INDEX_SWITCH_BUMPER_FRONT   5
#define INDEX_SWITCH_BUMPER_RIGHT   6

typedef enum
{
    TYPE_INPUT_SENSOR = 0x00,
    TYPE_INPUT_SWITCH

} TYPE_INPUT;

//*********************************** TYPE ***********************************

typedef enum
{
    TYPE_SENSOR_NULL = 0x00,
    TYPE_SENSOR_IR,
    TYPE_SENSOR_LIGHT,
    TYPE_SENSOR_SONAR,

    TYPE_SENSOR_NUM
} TYPE_SENSOR;

static const char *g_sensorTypeDis[TYPE_SENSOR_NUM] =
{
/* 0x00 */  "null",
/* 0x01 */  "IR",
/* 0x02 */  "light",
/* 0x03 */  "sonar"
};

static inline const char *GetSensorTypeChar(TYPE_SENSOR type)
{
    return g_sensorTypeDis[type];
}

//*********************************** TYPE ***********************************
typedef enum
{
    SITE_NULL = 0x00,
    SITE_1,
    SITE_2,
    SITE_3,
    SITE_4,
    SITE_5

}SITE;


typedef enum
{
    TYPE_SWITCH_NULL = 0x00,
    TYPE_SWITCH_BUMPER,
    TYPE_SWITCH_WHISKER,

    TYPE_SWITCH_NUM
} TYPE_SWITCH;


static const char *g_swtichTypeDis[TYPE_SWITCH_NUM] =
{
/* 0x00 */  "null",
/* 0x01 */  "bumper",
/* 0x02 */  "whisker"
};

static inline const char *GetSwitchTypeChar(TYPE_SWITCH type)
{
    return g_swtichTypeDis[type];
}

//********************************* POSITION *********************************


typedef enum
{
    POSITION_NULL   = 0x00,
    POSITION_FRONT  = 0x01,
    POSITOIN_BACK   = 0x02,
    POSITION_LEFT   = 0x03,
    POSITION_RIGHT  = 0x04,
    POSITION_MIDDLE = 0x05,
    POSITION_UNDER  = 0x06,

    POSITION_NUM
} POSITION;

static const char *g_position[POSITION_NUM] =
{
/* 0x00 */  "null",
/* 0x01 */  "front",
/* 0x02 */  "back",
/* 0x03 */  "left",
/* 0x04 */  "right",
/* 0x05 */  "middle",
/* 0x06 */  "under"
};

static inline const char *GetPositionChar(POSITION position)
{
    if (position < POSITION_NUM)
    {
        return g_position[position];
    }
    else
    {
        return "unknown position";
    }
}


//****************************Frequency*****************************
typedef enum
{
    FREQUENCY_NULL = 0x00000000,
    FREQUENCY_HALF,
    FREQUENCY_1,
    FREQUENCY_2,
    FREQUENCY_4,
    FREQUENCY_6,
    FREQUENCY_8
}FREQUENCY;
//*************************** Event ********************************


typedef struct
{
    TYPE_INPUT type;
    int subType;

    int index;
    int value;

    POSITION pos;
} INPUT;

typedef enum
{
    EVENT_NULL = 0x00000000,

    EVENT_HIT,
    EVENT_HIT_FRONT,
    EVENT_HIT_FRONT_LEFT,
    EVENT_HIT_FRONT_RIGHT,

    EVENT_DETECT,
    EVENT_DETECT_BLACK,
    EVENT_DETECT_GROUND,

    EVENT_LIGHT,
    EVENT_TRIGGER_ACTIVATED,

    EVENT_NUM
} EVENT;

static const char *g_eventDis[EVENT_NUM] =
{
/* 0x00000000 */    "event null",
/* 0x00000001 */    "event hit",
/* 0x00000002 */    "event hit front",
/* 0x00000003 */    "event hit front left",
/* 0x00000004 */    "event hit front right",
/* 0x00000005 */    "event detect",
/* 0x00000006 */    "event detect balck",
/* 0x00000007 */    "event detect ground",
/* 0x00000008 */    "event light"
};

static inline const char *GetEventChar(EVENT event)
{
    if (event < EVENT_NUM)
    {
        return g_eventDis[event];
    }
    else
    {
        return "unknown event";
    }
}


typedef enum
{
    STATUS_ROBOT_NULL = 0x00000000,
    STATUS_ROBOT_EXPLORING,
    STATUS_ROBOT_DOCKING,
    STATUS_ROBOT_DETECTING_FREQUENCY,
    STATUS_TRAINING,

    STATUS_ROBOT_NUM
} STATUS_ROBOT;

static const char *g_statusDis[STATUS_ROBOT_NUM] =
{
/* 0x00000000 */    "status null",
/* 0x00000001 */    "status EXPLORING",
/* 0x00000002 */    "status DOCKING",
};

static inline const char *GetStatusChar(STATUS_ROBOT status)
{
    return g_statusDis[status];
}

#define INDEX_MOTOR_LEFT    1
#define INDEX_MOTOR_RIGHT   0

#endif // CONF_H_INCLUDED
