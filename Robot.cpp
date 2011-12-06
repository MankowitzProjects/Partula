#include "Robot.h"
#include "Handle.h"
#include "Event.h"

extern SensorController g_sensorCtrl;
extern MotorController g_motorCtrl;
extern Event g_eventCenter;
//Declare the event and robot status variables
STATUS_ROBOT robotStatus;
extern EVENT currentEvent;

Robot::Robot(void)
{
    prt_debug("Creating robot\n");
    isInit = false;
}

Robot::~Robot(void)
{
    ;
}

bool Robot::hasHitBumper()
{

    return (currentEvent == EVENT_HIT_FRONT|| currentEvent==EVENT_HIT_FRONT_LEFT || currentEvent==EVENT_HIT_FRONT_RIGHT);
}

void Robot::run(void)
{

    g_motorCtrl.setAcc(100.00);
    g_motorCtrl.setVel(100.00);
    Handle handle;
    Event event;
    currentEvent = EVENT_NULL;

    printf("Robot::run\n");
    while (1)
    {

//      INPUT switchInput;
//        switchInput.type  = TYPE_INPUT_SWITCH;
//        switchInput.index = INDEX_SWITCH_BUMPER_LEFT ;
//        switchInput.value = STATE_ON;
//
//        g_sensorCtrl.setSwitchState(INDEX_SWITCH_BUMPER_LEFT , STATE_ON);
//
//
//        g_eventCenter.handleInput(switchInput);
        if(robotStatus == STATUS_ROBOT_EXPLORING)
        {

            if (hasHitBumper())
            {

                handle.collision();


            }
            else if (currentEvent == EVENT_DETECT_BLACK)
            {

                handle.docking();

            }
            else if (currentEvent == EVENT_TRIGGER_ACTIVATED)
            {

                handle.localization();
            }

        }else if(robotStatus==STATUS_ROBOT_DOCKING)
        {

            if (hasHitBumper())
            {

                handle.triggerSwitch();


            }

        }


        //Pose.updatePose();

    }


    /*   EVENT e = EVENT_NULL;

       e = GenEvent(TYPE_INPUT_SWITCH, INDEX_SWITCH_BUMPER_LEFT, STATE_ON);
       HandleEvent(e);

       e = GenEvent(TYPE_INPUT_SWITCH, INDEX_SWITCH_BUMPER_RIGHT, STATE_ON);
       HandleEvent(e);

       e = GenEvent(TYPE_INPUT_SWITCH, INDEX_SWITCH_BUMPER_FRONT, STATE_ON);
       HandleEvent(e);

       g_sensorCtrl.setSensorValue(INDEX_SENSOR_LIGHT_UNDER, 120);
       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 20);
       HandleEvent(e);

       g_sensorCtrl.setSensorValue(INDEX_SENSOR_LIGHT_UNDER, 130);
       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 130);
       HandleEvent(e);

       g_sensorCtrl.setSensorValue(INDEX_SENSOR_LIGHT_UNDER, 150);
       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 150);
       HandleEvent(e);

       g_sensorCtrl.setSensorValue(INDEX_SENSOR_LIGHT_UNDER, 121);
       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 121);
       HandleEvent(e);

       g_sensorCtrl.setSensorValue(INDEX_SENSOR_LIGHT_UNDER, 140);
       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 140);
       HandleEvent(e);

       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 130);
       HandleEvent(e);

       e = GenEvent(TYPE_INPUT_SENSOR, INDEX_SENSOR_LIGHT_UNDER, 250);
       HandleEvent(e);
     */
    printf("Robot::run - DONE~!\n");
}

void Robot::init(void)
{
    if (isInit)
    {
        return;
    }

    g_motorCtrl.init();
    g_sensorCtrl.init();

    isInit = true;
}

void Robot::fin(void)
{
    printf("Robot::fin\n");

    g_motorCtrl.fin();
    g_sensorCtrl.fin();
}
