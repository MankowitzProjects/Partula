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


#define VALUE_DIFF_IR_TOP_BOTTOM 20

#define VALUE_DIFF_GAP_TOP_BOTTOM   80
#define VALUE_DIFF_GAP              30
#define VALUE_DIFF_EDGE             20

#define VALUE_DIV_IR                13

bool bIsEdge(float irBottom, float irBottomPre)
{
    // if the value changes a lot
    if ((irBottomPre - irBottom) > VALUE_DIFF_EDGE)
    {
        if (irBottom < 90)
        {
            return true;
        }
    }

    return false;
}

bool bIsGap(float irTop, float irTopPre, float irBottom, float irBottomPre)
{
    // if it is the gap, top sensor will have very low reading compare to previous value
    // the bottom one will have similar value like before

    // the previous value should be similar
    if (   ((irTopPre - irTop) > VALUE_DIFF_GAP)
        && (abs(irBottomPre - irBottom) < VALUE_DIV_IR)
        && (irBottom - irTop) > 60)
    {
        return true;
    }

    return false;
}

void Handle::reScan(){

    int scanParam=0;
    pthread_t scanningThread;
    int rc = pthread_create(&scanningThread, NULL, &scanArea,(void*)&scanParam);
    cout << "reScanning - create" << rc << endl;

}
//****************************************************SCANNINGTHREAD****************************************

/** \brief Perform a scan in order to detect the center of the resource site
 *
 * \param param void*
 * \return void*
 *
 */
void* Handle::scanArea(void* param)
{
    // change the status to find the trigger
    robotStatus = STATUS_ROBOT_FINDING_TRIGGER;
    cout<<"scanArea: Entered Scan Area"<<endl;
    cout<<"scanArea: Center the sonar"<<endl;
    g_servoCtrl.setPos(VALUE_SERVO_POS_MID);

    ActStop();

    #if DEBUG_MODE_SONAR_SCAN
    cout << "scanArea - turn left at start" <<  endl;
    #endif
    wait(2000);

    turnLeft();

#if 1
    //leftDirectionDistance = irMainValueBottom;
    bool foundFlag    = false;
    bool edgeDetected = false;

    double irMainValueTop    = 0;
    double irMainValueBottom = 0;

    double irTopPrevious;
    double irBottomPrevious;

    // The centre is not found
    // The edge is not detected
    while ((!foundFlag) || (!edgeDetected))
    {
        irMainValueTop    = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irMainValueBottom = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_BOTTOM);

        if (irMainValueTop != irTopPrevious)
        {
            cout<<"IR Top Value: "<<irMainValueTop<<endl;
            irTopPrevious = irMainValueTop;
        }

        if (irMainValueBottom != irBottomPrevious){
            cout<<"IR Bottom Value: "<<irMainValueBottom<<endl;
            irBottomPrevious = irMainValueBottom;
        }

        // if it is the gap
        if (abs(irMainValueBottom - irMainValueTop) > 120)
        {
            cout<<"Stopped due to gap in first loop"<<endl;
            ActStop();
            foundFlag=true;
            break;
        }
        // it is the edge
        else if ((irMainValueTop < 90) && (irMainValueBottom < 90))
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
            else if (   (irMainValueTop    < 90)
                     && (irMainValueBottom < 90))
            {
                cout<<"Stopped due to edge in second loop"<<endl;
                foundFlag=false;
                ActStop();

                break;
            }
        }
    }
    cout<<"Reached Here"<<endl;
    //Go forward to center
    moveForward();

#else
    bool bFoundGap       = false;
    bool bFoundLeftEdge  = false;
    bool bFoundRightEdge = false;

    float irTop       = 0.0;
    float irTopPre    = 0.0;
    float irBottom    = 0.0;
    float irBottomPre = 0.0;

    // turn to the left edge until found the gap or edge
    while ((!bFoundGap) || (!bFoundLeftEdge))
    {
        irTop       = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irBottom    = g_sensorCtrl.getSensorValuePre(INDEX_SENSOR_IR_BOTTOM);
        irTopPre    = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
        irBottomPre = g_sensorCtrl.getSensorValuePre(INDEX_SENSOR_IR_BOTTOM);

        // detect if it's the edge using the only bottom sensor
        if (bIsEdge(irBottom, irBottomPre))
        {
            #if DEBUG_MODE_SONAR_SCAN
            cout << "scanArea - 1st loop found edge: irBottom = " << irBottom << " irBottomPre = " << irBottomPre <<  endl;
            #endif
            bFoundLeftEdge = true;
            break;
        }

        #if 1
        // detect if it's a gap
        if (bIsGap(irTop, irTopPre, irBottom, irBottomPre))
        {
            #if DEBUG_MODE_SONAR_SCAN
            cout << "scanArea - 1st loop found gap: irBottom = " << irBottom << " irBottomPre = " << irBottomPre << " irTop = " << irTop << " irTopPre = " << irTopPre <<  endl;
            #endif
            bFoundGap = true;
            break;
        }
        #endif
    }

    #if DEBUG_MODE_SONAR_SCAN
    cout << "scanArea - stop motor after 1st loop" <<  endl;
    #endif
    ActStop();

    // if facing to the left edge
    if (!bFoundGap) // that means we didn't found the gap
    {
        // turn to right
        #if DEBUG_MODE_SONAR_SCAN
        cout << "scanArea - turn right, start 2nd loop" <<  endl;
        #endif
        turnRight();
        wait(100);

        while ((!bFoundGap) || (!bFoundRightEdge))
        {
            irTop       = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
            irBottom    = g_sensorCtrl.getSensorValuePre(INDEX_SENSOR_IR_BOTTOM);
            irTopPre    = g_sensorCtrl.getSensorValue(INDEX_SENSOR_IR_TOP);
            irBottomPre = g_sensorCtrl.getSensorValuePre(INDEX_SENSOR_IR_BOTTOM);

            // detect if it's the edge using the only bottom sensor
            if (bIsEdge(irBottom, irBottomPre))
            {
                #if DEBUG_MODE_SONAR_SCAN
                cout << "scanArea - 2nd loop found edge: irBottom = " << irBottom << " irBottomPre = " << irBottomPre <<  endl;
                #endif
                bFoundRightEdge = true;
                break;
            }

            // detect if it's a gap
            if (bIsGap(irTop, irTopPre, irBottom, irBottomPre))
            {
                #if DEBUG_MODE_SONAR_SCAN
                cout << "scanArea - 2nd loop found gap" << endl;
                #endif
                bFoundGap = true;
                break;
            }
        }

        #if DEBUG_MODE_SONAR_SCAN
        cout << "scanArea - stop motor after 2nd loop" <<  endl;
        #endif
        ActStop();

        // what if only edge is found?
        if (bFoundRightEdge)
        {
            cout << "Only edge is found" << endl;
        }
    }

    // done, now the robot should be facing either the centre or the "right" edge

    #if DEBUG_MODE_SONAR_SCAN
    cout << "scanArea - done, now the robot should be facing either the centre or the right edge" <<  endl;
    #endif

#endif

    pthread_exit(NULL);
}

#define DIV_FREQUENCY_HALF 0.2
#define FREQUENCY_

//Checks the frequency when the robot has reached the trigger
void* Handle::fr_check(void* param)
{
    // wait for 2 seconds, until the robot get the frequency
    wait(4500);

    // get the sensible value from the front sensors
    float frequency = FreqGetFrequency();


    cout<<"The frequency is " << frequency << endl;

    //Update the site status, and set the robots position to the site position
    //then do frequency movements
    if(frequency>0 && frequency<10)
    {
     g_localization.updateSiteStatus();
    }

    if (frequency>0 && frequency <1)
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


