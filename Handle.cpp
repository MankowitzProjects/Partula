/*
 * File:   Handle.cpp
 * Author: daniel
 *
 * Created on December 3, 2011, 9:44 PM
 */

#include "Handle.h"
#include "SensorController.h"
#include "Localization.h"

extern EVENT currentEvent;
extern SensorController g_sensorCtrl;
extern ServoController g_servoCtrl;
extern STATUS_ROBOT robotStatus;
extern Localization g_localization;

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
    cout<<"docking() - Detected sensor value: "<<g_sensorCtrl.getSensorValue(INDEX_SENSOR_LIGHT_UNDER)<<endl;
    //Black tape has been detected
    //1. PerformShape Detection
    cout<<"docking() Black Tape Detected"<<endl;
    cout<<"docking() Robot is docking"<<endl;
    //navigSpeed=50.00;
    cout<<"docking() scanning Thread started"<<endl;
    int scanParam=0;
    ActStop();
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

    robotStatus = STATUS_ROBOT_FINDING_TRIGGER;
    cout<<"scanArea: Entered Scan Area"<<endl;

    cout<<"scanArea: Center the sonar"<<endl;

    g_servoCtrl.setPos(130);

    cout<<"scanArea: Turning left"<<endl;
    turnLeft();
    //leftDirectionDistance = irMainValueBottom;
    bool foundFlag=false;
    bool edgeDetected=false;

    double irMainValueTop = 0;
    double irMainValueBottom=  0;

    double irTopPrevious;
    double irBottomPrevious;

    // The centre is not found
    // The edge is not detected
    while ((!foundFlag) || (!edgeDetected))
    {
        irMainValueTop    = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

        if (irMainValueTop!=irTopPrevious){
            cout<<"IR Top Value: "<<irMainValueTop<<endl;
            irTopPrevious = irMainValueTop;
        }

        if(irMainValueBottom != irBottomPrevious){
            cout<<"IR Bottom Value: "<<irMainValueBottom<<endl;
            irBottomPrevious = irMainValueBottom;
        }

        // if it is the gap
        if( abs(irMainValueBottom - irMainValueTop) >120)
        {
            cout<<"Stopped due to gap in first loop"<<endl;
            ActStop();
            foundFlag=true;
            break;
        }
        // it is the edge
        else if((irMainValueTop < 140) && (irMainValueBottom<140))
        {
            cout<<"Stopped due to edge in first loop"<<endl;
            ActStop();
            edgeDetected=true;
            break;
        }
        else
        {
            ;
        }
    }

    //printf("Left direction Distance is %d",leftDirectionPrevious);

    if(foundFlag==false)
    {
        cout<<"Turning right"<<endl;
        turnRight();

        //rightDirectionDistance = irMainValueBottom;
        while(!(foundFlag))
        {

            irMainValueTop = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
            irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

            if(abs(irMainValueBottom - irMainValueTop)>120)
            {
                cout<<"Stopped due to gap in second loop"<<endl;
                ActStop();
                ActTurnRight(300);
                foundFlag=true;
                break;

            }
            else if(irMainValueTop<140 && irMainValueBottom<140)
            {
                cout<<"Stopped due to edge in second loop"<<endl;
                foundFlag=false;
                ActStop();
                break;

            }

        }



    }

    moveForward();
    robotStatus = STATUS_ROBOT_DOCKING;

    pthread_exit(NULL);

}


//Checks the frequency when the robot has reached the trigger
void* Handle::fr_check(void* param)
{


    int up,down,counter;


    //*****************************
    timeval starttime;
    int check_frequency;
    double frequency = 0;
    int LightValue=0;
    //*****************************
    // while(lightFound==false){};
    //lightFound=false;
    g_servoCtrl.setPos(130);

    printf(" Check frequency\n");
    check_frequency=1;
    gettimeofday(&starttime,NULL);

    up=0;
    timeval currenttime;
    timeval strttime;
    gettimeofday(&currenttime,NULL);

    while (( ((currenttime.tv_sec * 1000000) + (currenttime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec))  ) < 4000000)
    {

        //TODO Get raw light value readings.

        if (LightValue>400&&(up==0))
        {
            //  printf("Up detected");
            up=1;
        }
        else if (LightValue<100&&(up==1))
        {
            counter++;
            //printf("Down detected");
            up=0;
        }
        gettimeofday(&currenttime,NULL);
    }

    //Need to divide frequency by 2

    frequency = (double)counter/4;
    //printf("\n\n\n\nThe counter is \n%d\n\n\n\n",counter);
    //printf("\n\n\n\nThe frequency is \n%f\n\n\n\n",frequency);
    cout<<"The frequency is "<<frequency<<endl;

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

    robotStatus=STATUS_ROBOT_EXPLORING;

    check_frequency=0;
    frequency=0;



    pthread_exit(NULL);

}


