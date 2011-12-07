#include "Robot.h"
#include "Handle.h"
#include "Event.h"
#include "Pose.h"
#include "Localization.h"

extern SensorController g_sensorCtrl;
extern MotorController g_motorCtrl;
extern ServoController g_servoCtrl;
extern Event g_eventCenter;
//Declare the event and robot status variables
STATUS_ROBOT robotStatus;
extern EVENT currentEvent;
extern Localization g_localization;

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
    return (   (currentEvent == EVENT_HIT_FRONT)
            || (currentEvent == EVENT_HIT_FRONT_LEFT)
            || (currentEvent == EVENT_HIT_FRONT_RIGHT));
}

void Robot::run(void)
{

    g_motorCtrl.setAcc(100.00);
    g_motorCtrl.setVel(100.00);
    Handle handle;
    Event event;
    robotStatus  = STATUS_ROBOT_EXPLORING;
    currentEvent = EVENT_NULL;

    //Set the initial position
    g_localization.initializePosition(0,0,0);

    cout<<"Robot::run"<<endl;

    INPUT switchInput;
    switchInput.type    = TYPE_INPUT_SWITCH;
    switchInput.subType = g_sensorCtrl.getSwitchType(INDEX_SWITCH_BUMPER_LEFT);
    switchInput.index   = INDEX_SWITCH_BUMPER_LEFT;
    switchInput.value   = STATE_ON;
    switchInput.pos     = POSITION_FRONT;

    g_sensorCtrl.setSwitchState(INDEX_SWITCH_BUMPER_LEFT, STATE_ON);

    g_eventCenter.handleInput(switchInput);

    while (true)
    {
        ;
    }

    #if 0
    while (1)
    {
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
        }
        else if(robotStatus==STATUS_ROBOT_DOCKING)
        {
            if (hasHitBumper() && robotStatus!=STATUS_ROBOT_DETECTING_FREQUENCY)
            {
                handle.triggerSwitch();
            }
        }
        //Pose.updatePose();
    }
    #endif

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
    //g_servoCtrl.init();

    isInit = true;
}

void Robot::fin(void)
{
    printf("Robot::fin\n");

    g_motorCtrl.fin();
    g_sensorCtrl.fin();
    //g_servoCtrl.fin();
}
