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
ServoController g_servoController;
extern STATUS_ROBOT robotStatus;

Handle::Handle()
{

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
    //navigSpeed=50.00;
    cout<<"Thread cancelled"<<endl;
    int scanParam=0;
    pthread_t scanningThread;
    pthread_create(&scanningThread, NULL, &scanArea,(void*)&scanParam);
}

void Handle::localization()
{



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
    ActTurnLeft(0);
    //leftDirectionDistance = irMainValueBottom;
    bool foundFlag=false;
    bool edgeDetected=false;

    double irMainValueTop;
    double irMainValueBottom;

    while(foundFlag==false && edgeDetected==false)
    {

        irMainValueTop = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

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
        ActTurnRight(1000);

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
    ActMoveForward(0);

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
    g_servoController.setPos(130);

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

    robotStatus=STATUS_ROBOT_EXPLORING;

    check_frequency=0;
    frequency=0;



    pthread_exit(NULL);

}


