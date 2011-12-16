#include "Robot.h"
#include "Handle.h"
#include "Event.h"
#include "Pose.h"
#include "Localization.h"
#include "Action.h"
#include "Frequency.h"
#include "Site.h"

extern SensorController g_sensorCtrl;
extern MotorController g_motorCtrl;
extern ServoController g_servoCtrl;
extern Event g_eventCenter;
//Declare the event and robot status variables
STATUS_ROBOT robotStatus;
extern EVENT currentEvent;
extern Localization g_localization;
extern MOVEMENT_STATUS g_movement;
EVENT g_event;
extern MOVEMENT_STATUS g_prev_movement;
Pose pose(0,0,0);

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
//Use this thread to check for changes in the robots movement and update the odometry accordingly
void * updatingOdometry(void * param){

   while(1){

       if (g_movement!=g_prev_movement){
           pose.updateOdometry();
       }
    }
}

//The main control loop
void Robot::run(void)
{

    Handle handle;
    Event event;
    robotStatus = STATUS_ROBOT_EXPLORING;
    currentEvent = EVENT_NULL;

    g_servoCtrl.setPos(VALUE_SERVO_POS_MID);

    //Set the initial position
    g_localization.initializePosition(0,0,0);

    cout<<"Robot::run"<<endl;
    
    sonarScanSite();

#if 1
    int param=1;
    int i=1;
    pthread_t odometryThread;
    pthread_create(&odometryThread, NULL, updatingOdometry,(void*)&param);

    cout<<"Finished scan, moving forward"<<endl;
    ActMoveForward(6000);
    trainSensors();

    while (1)
    {
        //If the robot is exploring, run this set of behaviours
        if(robotStatus == STATUS_ROBOT_EXPLORING)
        {
            //cout<<"Entered Exploring"<<endl;
            if (hasHitBumper())
            {
                cout<<"Handling collision"<<endl;
                handle.collision();
            }
            else if (currentEvent == EVENT_DETECT_BLACK)
            {
                cout<<"Handling Docking"<<endl;
                handle.docking();
            }
            else if (currentEvent == EVENT_TRIGGER_ACTIVATED)
            {
                cout<<"Handling localization"<<endl;
                handle.localization();
            }
            else if(   (currentEvent == EVENT_LOCALIZING)
                    && (g_movement   == STOPPED))
            {
                handle.reScan();
            }
            else
            {
                ;
            }
        }
		//If the robot is docking, run this set of behaviours
        else if (robotStatus == STATUS_ROBOT_DOCKING)
        {
            if (    hasHitBumper()
                && (robotStatus != STATUS_ROBOT_DETECTING_FREQUENCY))
            {
               g_event= currentEvent;
                cout<<"Robot: The current event is: "<<currentEvent<<" and the event is: "<<g_event<<endl;
                handle.triggerSwitch();
            }

        }
        else
        {
            ;
        }
    }

#endif

    printf("Robot::run - DONE~!\n");
}
//Initialisation procedure for the motors, sensors and servo
void Robot::init(void)
{
    if (isInit)
    {
        return;
    }

    initSites();
    cout<<"Intialised Sites"<<endl;
    g_motorCtrl.init();
    g_sensorCtrl.init();
    g_servoCtrl.init();


    isInit = true;
}
//Called when the program has ended
void Robot::fin(void)
{
    printf("Robot::fin\n");

    g_motorCtrl.fin();
    g_sensorCtrl.fin();
    g_servoCtrl.fin();
}
//The training algorithm used to determine the black tape threshold
void Robot::trainSensors()
{

    int lightSensor =0;
    int numberSamples=0;
    int lightTotal = 0;

    while(numberSamples<100)
    {
        lightSensor = g_sensorCtrl.getSensorValue(INDEX_SENSOR_LIGHT_UNDER);

        if(lightSensor>100){

            //cout<<"Accepted light value: "<<lightSensor<<endl;
            lightTotal = lightTotal + lightSensor;
            numberSamples++;
        }
    }

    lightTotal = lightTotal/numberSamples;

    VALUE_BLACK_TAPE_MAX = lightTotal*0.6;
    VALUE_BLACK_TAPE_MIN = VALUE_BLACK_TAPE_MAX*0.4;

}
