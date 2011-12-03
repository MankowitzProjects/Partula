#include "Robot.h"
#include "Action.h"

extern SensorController g_sensorCtrl;
extern MotorController  g_motorCtrl;

Robot::Robot(void)
{
    prt_debug("Creating robot\n");
    isInit = false;
}

Robot::~Robot(void)
{
    ;
}

void Robot::run(void)
{

    g_motorCtrl.setAcc(100.00);
    g_motorCtrl.setVel(100.00);
  
    printf("Robot::run\n");
	while(1){
	
	EVENT = handlevent();
	genActandleCollision();
	act
	
	if(EVENT == COLLISION)
	{
	HandleCollision();
	}else if(EVENT==BLACKTAPE)
	handleBlackTape();
	}
	else if (EVENT== TRIGGERACTIVATED)
	{
	handleLocalisation();
	}
	
	updatePose();
	
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
