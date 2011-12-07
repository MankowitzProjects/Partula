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

    /*INPUT switchInput;
    switchInput.type    = TYPE_INPUT_SWITCH;
    switchInput.subType = g_sensorCtrl.getSwitchType(INDEX_SWITCH_BUMPER_LEFT);
    switchInput.index   = INDEX_SWITCH_BUMPER_LEFT;
    switchInput.value   = STATE_ON;
    switchInput.pos     = POSITION_FRONT;

    g_sensorCtrl.setSwitchState(INDEX_SWITCH_BUMPER_LEFT, STATE_ON);

    g_eventCenter.handleInput(switchInput);*/


   // while (true)
    //{
    //    ;
    //}

    //trainSensors();

    #if 1
    while (1)
    {
        //cout<<"Current Event is %d"<<currentEvent<<endl;
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

    void Robot::trainSensors(){
	      robotStatus=STATUS_TRAINING;
	      int samples = 0;
	      int blackTapeTotal=0;
	      int blackTapeAvg=0;
          int lightReading = 0;
	      int lightReadingPrevious=0;
	      cout<<"Press to start training"<<endl;
	      getchar();
	      while(samples<5){
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
