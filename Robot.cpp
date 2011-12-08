#include "Robot.h"
#include "Handle.h"
#include "Event.h"
#include "Pose.h"
#include "Localization.h"
#include "Action.h"

#include "Site.h"


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

#include "Frequency.h"

void Robot::run(void)
{


    Handle handle;
    Event event;
    robotStatus  = STATUS_ROBOT_SONAR_SCANNING;
    currentEvent = EVENT_NULL;

    //Set the initial position
    g_localization.initializePosition(190,120,0);

    //g_servoCtrl.setPos(130);
    cout<<"Robot::run"<<endl;

    //trainSensors();

    //moveForward();
    //turnRight();

#if 1

    int sonarStatus = 0;
    pthread_t sonarThread;
    pthread_create(&sonarThread, NULL, g_localization.sonarScan,(void*)&sonarStatus);

    while(robotStatus==STATUS_ROBOT_SONAR_SCANNING){

    }
    cout<<"Finished scan, moving forward"<<endl;

    ActMoveForward(6000);



    while (1)
    {
        //cout<<"Light under car is: "<<g_sensorCtrl.getSensorValue(INDEX_SENSOR_LIGHT_UNDER)<<endl;
        //cout<<"Current Event is %d"<<currentEvent<<endl;
        if(robotStatus == STATUS_ROBOT_EXPLORING)
        {
            //cout<<"Entered Exploring"<<endl;
            if (hasHitBumper())
            {
                //cout<<"Handling collision"<<endl;
                handle.collision();
            }
            else if (currentEvent == EVENT_DETECT_BLACK)
            {
                //cout<<"Handling Docking"<<endl;
                handle.docking();
            }
            else if (currentEvent == EVENT_TRIGGER_ACTIVATED)
            {
                //cout<<"Handling localization"<<endl;
                handle.localization();
            }
        }
        else if(robotStatus==STATUS_ROBOT_DOCKING)
        {
            if (hasHitBumper() && robotStatus!=STATUS_ROBOT_DETECTING_FREQUENCY)
            {
                cout<<"Handling trigger switch"<<endl;
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

    //initSites();

    g_motorCtrl.init();
    g_sensorCtrl.init();
    g_servoCtrl.init();

    isInit = true;
}

void Robot::fin(void)
{
    printf("Robot::fin\n");

    g_motorCtrl.fin();
    g_sensorCtrl.fin();
    g_servoCtrl.fin();
}

void Robot::trainSensors()
{
    robotStatus=STATUS_TRAINING;
    int samples = 0;
    int blackTapeTotal=0;
    int blackTapeAvg=0;
    int lightReading = 0;
    int lightReadingPrevious=0;
    cout<<"Press to start training"<<endl;
    getchar();
    while(samples<5)
    {
        lightReading = g_sensorCtrl.getSensorValue(INDEX_SENSOR_LIGHT_UNDER);

        if(lightReading!=lightReadingPrevious)
        {
            cout<<"Light Reading Trainer Value"<<lightReading<<endl;
            lightReadingPrevious = lightReading;
            blackTapeTotal = blackTapeTotal+lightReading;
            samples++;
        }
    }
    blackTapeAvg = blackTapeTotal/(samples+1);
    VALUE_BLACK_TAPE_MAX = blackTapeAvg;
    VALUE_BLACK_TAPE_MIN = blackTapeAvg - 60;

    cout<<"Black Tape Threshold max threshold is "<<VALUE_BLACK_TAPE_MAX<<endl;
    cout<<"Black Tape Threshold min threshold is "<<VALUE_BLACK_TAPE_MIN<<endl;
    getchar();
    robotStatus=STATUS_ROBOT_EXPLORING;

}

