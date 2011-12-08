/*
 * File:   Handle.cpp
 * Author: daniel
 *
 * Created on December 3, 2011, 9:44 PM
 */

#include "Handle.h"
#include "SensorController.h"
#include "Localization.h"
#include "Frequency.h"

extern EVENT currentEvent;
extern SensorController g_sensorCtrl;
extern ServoController g_servoCtrl;
extern STATUS_ROBOT robotStatus;
extern Localization g_localization;
extern MotorController g_motorCtrl;

Handle::Handle()
{
    //Pose pose;
}

Handle::Handle(const Handle& orig)
{
}

Handle::~Handle()
{
}

//Handles collisions
void Handle::collision()
{
    hitBumper(1000);
    currentEvent = EVENT_NULL;
}

void Handle::docking()
{
    //Set the robot status to docking
    robotStatus = STATUS_ROBOT_DOCKING;

    //Black tape has been detected
    //1. PerformShape Detection
    cout<<"Black Tape Detected"<<endl;
    cout<<"Robot is docking"<<endl;
    //navigSpeed=50.00;
    cout<<"Thread cancelled"<<endl;
    int scanParam=0;
    pthread_t scanningThread;
    pthread_create(&scanningThread, NULL, &scanArea,(void*)&scanParam);
}

void Handle::localization()
{
    ;
}

void Handle::triggerSwitch()
{

    robotStatus = STATUS_ROBOT_DETECTING_FREQUENCY;
    int frequencyParam=0;
    //Start a thread to check the frequency
    pthread_t thr;
    pthread_create(&thr, NULL, fr_check,(void*)&frequencyParam);
}

//****************************************************SCANNINGTHREAD****************************************

//Perform a scan in order to detect the center of the resource site
void* Handle::scanArea(void* param)
{
    cout<<"Entered Scan Area"<<endl;

    cout<<"Center the sonar"<<endl;
    
    g_servoCtrl.setPos(130);
    
    ActTurnLeft(5000);

    //leftDirectionDistance = irMainValueBottom;
    bool foundFlag=false;
    bool edgeDetected=false;

    double irMainValueTop = 0;
    double irMainValueBottom=  0;

    double irTopPrevious;
    double irBottomPrevious;

    while(foundFlag==false && edgeDetected==false)
    {
        //cout<<"Stuck"<<endl;
        irMainValueTop = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

        if(irMainValueTop!=irTopPrevious){
        cout<<"IR Top Value: "<<irMainValueTop<<endl;
        irTopPrevious = irMainValueTop;
        }
        if(irMainValueBottom != irBottomPrevious){
        cout<<"IR Bottom Value: "<<irMainValueBottom<<endl;
        irBottomPrevious = irMainValueBottom;
        }

        if(irMainValueTop<90 && irMainValueBottom>90)
        {
            cout<<"Stopped due to gap in first loop"<<endl;
            ActStop();
            foundFlag=true;
        }
        else if(irMainValueTop<90 && irMainValueBottom<90)
        {
            cout<<"Stopped due to edge in first loop"<<endl;
            ActStop();
            edgeDetected=true;

        }
    }

    //printf("Left direction Distance is %d",leftDirectionPrevious);

    if(foundFlag==false)
    {
        ActTurnRight(5000);

        //rightDirectionDistance = irMainValueBottom;
        while(foundFlag==false)
        {
            irMainValueTop = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
            irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

            if(irMainValueTop<90 && irMainValueBottom>90)
            {
                cout<<"Stopped due to gap in second loop"<<endl;
                ActStop();
                ActTurnRight(300);
                foundFlag=true;

            }
            else if(irMainValueTop<90 && irMainValueBottom<90)
            {
                cout<<"Stopped due to edge in second loop"<<endl;
                ActStop();
            }
        }
    }
    cout<<"Reached Here"<<endl;
    //Go forward to center
    g_motorCtrl.setAcc(100.00);
    g_motorCtrl.setVel(100.00);

    pthread_exit(NULL);
}

#define DIV_FREQUENCY_HALF 0.2
#define FREQUENCY_

//Checks the frequency when the robot has reached the trigger
void* Handle::fr_check(void* param)
{
    // wait for 2 seconds, until the robot get the frequency
    wait(2000);

    float frequency = FreqGetFreqency();


    cout<<"The frequency is " << frequency << endl;

    //Once the robot knows the frequency, it can identify the site

    if(frequency>0 && frequency <1)
    {
        g_localization.identifySite(FREQUENCY_HALF);
        frequencyMovement(FREQUENCY_HALF);
    }
    else if(frequency >=0.8 && frequency <1.5)
    {
        g_localization.identifySite(FREQUENCY_1);
        frequencyMovement(FREQUENCY_1);
    }
    else if(frequency >=1.5 && frequency <3.5)
    {
        g_localization.identifySite(FREQUENCY_2);
        frequencyMovement(FREQUENCY_2);
    }
    else if(frequency >= 3.5 && frequency <5.5)
    {
        g_localization.identifySite(FREQUENCY_4);
        frequencyMovement(FREQUENCY_4);
    }
    else if(frequency >=5.5 && frequency <7.5)
    {
        g_localization.identifySite(FREQUENCY_6);
        frequencyMovement(FREQUENCY_6);
    }
    else if(frequency >=7.5 && frequency <12)
    {
        g_localization.identifySite(FREQUENCY_8);
        frequencyMovement(FREQUENCY_8);
    }

    currentEvent = EVENT_TRIGGER_ACTIVATED;
    robotStatus  = STATUS_ROBOT_EXPLORING;

    pthread_exit(NULL);
}


