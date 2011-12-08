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
    ActStop();
    cout<<"docking - Detected sensor value: "<<g_sensorCtrl.getSensorValue(INDEX_SENSOR_LIGHT_UNDER)<<endl;
    //Black tape has been detected
    //1. PerformShape Detection
    cout<<"docking - Black Tape Detected"<<endl;
    cout<<"docking - Robot is docking"<<endl;
    //navigSpeed=50.00;
    cout<<"docking - scanning Thread started"<<endl;

    int scanParam=0;
    pthread_t scanningThread;
    int rc = pthread_create(&scanningThread, NULL, &scanArea,(void*)&scanParam);
    cout << "docking - create" << rc << endl;
}

void Handle::localization()
{
    currentEvent = EVENT_LOCALIZING;

    g_localization.turnToFaceResourceSite();

    g_localization.takeMeasurements();

    g_localization.updateParticle();

    g_localization.moveToResourceSite();


}

void Handle::triggerSwitch()
{

    robotStatus = STATUS_ROBOT_DETECTING_FREQUENCY;
    int frequencyParam=0;
    //Start a thread to check the frequency
    pthread_t thr;
    pthread_create(&thr, NULL, fr_check,(void*)&frequencyParam);
}


void Handle::reScan(){

    int scanParam=0;
    pthread_t scanningThread;
    int rc = pthread_create(&scanningThread, NULL, &scanArea,(void*)&scanParam);
    cout << "reScanning - create" << rc << endl;

}
//****************************************************SCANNINGTHREAD****************************************

//Perform a scan in order to detect the center of the resource site
void* Handle::scanArea(void* param)
{
    cout<<"Entered Scan Area"<<endl;

    cout<<"Center the sonar"<<endl;

    g_servoCtrl.setPos(130);

    turnLeft();

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
            break;
        }

        // it is the edge
        else if((irMainValueTop < 90) && (irMainValueBottom < 90))
        {
            cout<<"Stopped due to edge in first loop"<<endl;
            ActStop();
            edgeDetected=true;
            break;

        }
    }

    //printf("Left direction Distance is %d",leftDirectionPrevious);

    if(foundFlag==false)
    {
        turnRight();

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
                break;

            }

            else if (   (irMainValueTop    < 90)
                     && (irMainValueBottom < 90))
            {
                cout<<"Stopped due to edge in second loop"<<endl;
                ActStop();

                //Hasn't found an edge
                //Move forward and scan
                break;
            }
        }
    }
    cout<<"Reached Here"<<endl;
    //Go forward to center
    moveForward();

    pthread_exit(NULL);
}

#define DIV_FREQUENCY_HALF 0.2
#define FREQUENCY_

//Checks the frequency when the robot has reached the trigger
void* Handle::fr_check(void* param)
{
    // wait for 2 seconds, until the robot get the frequency
    wait(2000);

    float frequency = FreqGetFrequencyMiddle();



    cout<<"The frequency is " << frequency << endl;

    //Update the site status, and set the robots position to the site position
    //then do frequency movements
    if(frequency>0 && frequency<10)
    {
     g_localization.updateSiteStatus();
    }

    if(frequency>0 && frequency <1)
    {

        frequencyMovement(FREQUENCY_HALF);
    }
    else if(frequency >=0.8 && frequency <1.5)
    {

        frequencyMovement(FREQUENCY_1);
    }
    else if(frequency >=1.5 && frequency <3.5)
    {

        frequencyMovement(FREQUENCY_2);
    }
    else if(frequency >= 3.5 && frequency <5.5)
    {

        frequencyMovement(FREQUENCY_4);
    }
    else if(frequency >=5.5 && frequency <7.5)
    {

        frequencyMovement(FREQUENCY_6);
    }
    else if(frequency >=7.5 && frequency <12)
    {

        frequencyMovement(FREQUENCY_8);
    }



    currentEvent = EVENT_TRIGGER_ACTIVATED;
    robotStatus  = STATUS_ROBOT_EXPLORING;

    pthread_exit(NULL);
}


