/*
 * File:   Localization.cpp
 * Author: daniel
 *
 * Created on December 3, 2011, 9:43 PM
 */

#include "Localization.h"


Localization g_localization;
extern Pose pose;
extern ServoController g_servoCtrl;
extern SensorController g_sensorCtrl;
extern STATUS_ROBOT robotStatus;
extern Site sites[NUM_SITE];
extern double velocity;
double sonarMeasurement;
double sonarMeasurementPosition;


Localization::Localization()
{

}

Localization::~Localization()
{

}

SITE siteIndex = SITE_1;

//Identifies the current site and updates the robots position
void Localization::updateSiteStatus()
{
    switch(siteIndex)
    {
        case SITE_1:
        {
            cout<<"The site has been set"<<endl;
            pose.setPose(142,220,M_PI/2);
            sites[siteIndex].bVisited=true;
            siteIndex = SITE_2;
            break;
        }
        case SITE_2:
        {
            pose.setPose(0,184,0);
            sites[siteIndex].bVisited=true;
            siteIndex = SITE_3;
            break;
        }
        case SITE_3:
        {
            pose.setPose(0,184,-M_PI/2);
            sites[siteIndex].bVisited=true;
            siteIndex = SITE_4;
            break;
        }
        case SITE_4:
        {
            pose.setPose(0,0,-M_PI/4);
            sites[siteIndex].bVisited=true;
            siteIndex = SITE_5;
            break;
        }
        case SITE_5:
        {
            pose.setPose(190,84,M_PI/2);
            sites[siteIndex].bVisited=true;
            siteIndex = SITE_5;
            break;
        }

        default:

        {

            break;
        }
    }
}

//Set the initial position of the robot
void Localization::initializePosition(double x, double y, double theta)
{
    //Sets the starting position of the robot
    pose.setPose(x,y,theta);

}

//Determine the resource site locations based on the current position
void Localization::turnToFaceResourceSite()
{
    DirTime directionTime;
    SITE currentSite;

    currentSite = siteIndex;
    cout<<"Site (x, y): ("<< sites[currentSite].readyPoint.x<<", "<<sites[currentSite].readyPoint.y<<")"<<endl;

    cout<<"Next site is: "<<currentSite<<endl;
    //Get the time that is needed to turn towards the next resource site
    directionTime = pose.shiftToGoal(currentSite);
    cout<<"Direction is : "<<directionTime.direction<<endl;
    cout<<"Time is: "<<directionTime.time<<endl;
    if (directionTime.direction == TURNING_LEFT)
    {
        //ActTurnLeft();
        turnLeft();
        wait(directionTime.time);
        stop();
    }
    else if(directionTime.direction == TURNING_RIGHT)
    {
        //ActTurnRight(directionTime.time);
        turnRight();
        wait(directionTime.time);
        stop();
    }

    //Calculate the distance to the resource site
    timeToTravelToResourceSite = pose.distanceToResourceSite(currentSite);
    cout<<"Time to travel to resource site: "<<timeToTravelToResourceSite<<endl;
}

extern MOVEMENT_STATUS g_movement;

//Take a measurement of the nearby obstacles
void Localization::takeMeasurements()
{
    //if (!bMeasureJobStarted)
    //{
     //   bMeasureJobStarted = true;

     //while(g_movement!=STOPPED){
//cout <<"NOT STOPPED"<<endl;
    // }

     cout << "STARTING TO TAKE MEASUREMENTS" << endl;

        //robotStatus = STATUS_ROBOT_TAKING_MEASUREMENTS;

        //int sonarStatus = 0;

       // pthread_t sonarThread;
        //pthread_create(&sonarThread, NULL, sonarScan,(void*)&sonarStatus);
        
        //
    //}


}

void Localization::updateParticle()
{
   

    cout<<"TAKING MEASUREMENTS"<<endl;
    //sonarMeasurement
    //sonarMeasurementPosition

    //pose.getPose

    //Get sonar distance in cm
    double sonarDistance = (sonarMeasurement/255)*645;

    predictedDistanceToResourceSite = timeToTravelToResourceSite*velocity;

    //Take the maximum of the two distances
    if((sonarDistanceToSite>predictedDistanceToResourceSite) && sonarDistanceToSite<400)
    {
    predictedDistanceToResourceSite = sonarDistance;

    }
    else if ((predictedDistanceToResourceSite>sonarDistanceToSite) && (predictedDistanceToResourceSite<400)){

    distanceToSite = predictedDistanceToResourceSite;

    }
    else if(predictedDistanceToResourceSite>400 && sonarDistanceToSite>400)
    {
        if(predictedDistanceToResourceSite>sonarDistanceToSite)
        {
            distanceToSite = predictedDistanceToResourceSite;
        }
        else{
        distanceToSite = sonarDistanceToSite;
        }

    }

    //Take the angular values
    double sonarAngle = sonarMeasurementPosition*(M_PI/220);
    double currenBearing = pose.getPose().theta;


    /*if(abs(sonarAngle-currenBearing)>(M_PI/4)){

        angleToSite = 0;

    }else{
        angleToSite = (sonarAngle)/4;

    }*/



    //If sonar value is very different from our predicted value, use sonar value.


    //


}

//Move the robot towards the resource site
void Localization::moveToResourceSite(){

//ActMoveForward(distanceToSite/velocity);
    moveForward();
    //wait(distanceToSite/velocity);
    wait(3000);
    stop();
    sonarScan();
    moveForward();
}

void Localization::sonarScan(void)
{
    cout<<"Entered Sonar Scanning Mode..."<<endl;
    //robotStatus = STATUS_ROBOT_SONAR_SCANNING;

    int firstServoMinPosition = -1;
    int lastServoMinPosition =-1;
    int finalServoMinPosition = 0;
    int servoPosition = -1;
    int minSonarValue = 1000;
    double minSonarPosition = -1.0;
    double currentPosition;
    double sonarValue;
    //The distance the servo has moved from the center position
    double servoOffset = 0.0;

    //The number of degrees that the robot needs to turn
    double degreeTurn=0.0;
    unsigned long turnMilisecs=0;
    //Find the nearest obstacle
    for (int i=0; i<220; i=i+5)
    {

        g_servoCtrl.setPos(i);
        //Get the current servo position
        currentPosition = g_servoCtrl.getPos();

        //Set the current sonar value
        sonarValue = g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR);
        //CPhidgetAdvancedServo_getPosition (servo, 0, &currentPosition);

        //Send the sonar data to a file
        //readSensorData(sonarValue);

        if (   (   (sonarValue <  minSonarValue+10)
                   || (sonarValue == minSonarValue))
                && (sonarValue>10))
        {
            //If the values are actually smaller
            if (sonarValue < minSonarValue)
            {
#if DEBUG_MODE_SONAR_SCAN
                cout<<"sonarScan - Resetting min position"<<endl;
#endif

                firstServoMinPosition =currentPosition;

                minSonarValue = sonarValue;

                //cout<<"Sonar Value: "<<currentPosition;
                minSonarPosition = currentPosition;

#if DEBUG_MODE_SONAR_SCAN
                cout<<"Min sonar value: "<<minSonarValue<<endl;
                cout<<"Min sonar Pos: "<<minSonarPosition<<endl;
#endif
            }

            if(sonarValue<minSonarValue+20 && sonarValue>minSonarValue-20)
            {
                cout<<"LAST POSITION CHANGED"<<endl;
                lastServoMinPosition = currentPosition;
            }
        }

        wait(200);

#if DEBUG_MODE_SONAR_SCAN
        cout<<"Sonar Position: "<<currentPosition<<endl;
        cout<<"Sonar Value: "<<g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR)<<endl;
#endif
    }


#if DEBUG_MODE_SONAR_SCAN
    cout<<"Initial Min Position: "<<firstServoMinPosition<<endl;
    cout<<"Final Min Position  : "<<lastServoMinPosition<<endl;
#endif

    finalServoMinPosition = (lastServoMinPosition+firstServoMinPosition)/2;
    minSonarPosition = finalServoMinPosition;

    g_servoCtrl.setPos(130);

    servoOffset = 120 - minSonarPosition;

#if DEBUG_MODE_SONAR_SCAN
    cout<<"Servo offset: "<<servoOffset;
    cout<<"Min Sonar value"<<minSonarValue<<endl;
    cout<<"Min Sonar position"<<minSonarPosition<<endl;
#endif

    //0.81 is the number of degrees that the servo turns after each step as the range
    //is from 0 to 220. Dividing 180 degrees by the number of steps yields the number
    //of degrees per servo step
    degreeTurn = servoOffset*0.81;

    if(servoOffset>0)
    {
#if DEBUG_MODE_SONAR_SCAN
        cout<<"servo offset greater than 0"<<endl;
#endif
        //How long the car must turn clockwise (in milisecs) to reach the current servo position.
        turnMilisecs = degreeTurn*16;

#if DEBUG_MODE_SONAR_SCAN
        cout<<"Number of degrees is "<<degreeTurn<<"and number of milisecs is "<<turnMilisecs<<endl;
#endif

        //ActTurnLeft(turnMilisecs);
        turnLeft();
        wait(turnMilisecs);

    }
    else if(servoOffset<0)
    {
#if DEBUG_MODE_SONAR_SCAN
        cout<<"servo offset less than 0"<<endl;
#endif

        //Note the turnMilisecs are unequal as the car requires slightly more time to turn clockwise
        turnMilisecs = degreeTurn*(-15);

#if DEBUG_MODE_SONAR_SCAN
        cout<<"Number of degrees is "<<degreeTurn<<"and number of milisecs is"<<turnMilisecs<<endl;
#endif

        //ActTurnRight(turnMilisecs);
        turnRight();
        wait(turnMilisecs);
    }

    //Move towards the new obstacle
    //ActMoveForward(0);
    sonarMeasurement = minSonarValue;
    sonarMeasurementPosition = minSonarPosition;

    robotStatus = STATUS_ROBOT_EXPLORING;
    //pthread_exit(NULL);
}

void Localization::readSensorData(int sensorValue)
{

    ofstream sonarReadings;
    sonarReadings.open("Sensor.txt", fstream::app);

    sonarReadings << "\nLocalization Sensor reading\n";
    for (int i=0; i<220; i=i+5)
    {
        g_servoCtrl.setPos(i);
        wait(200);
        sonarReadings << sensorValue;
        sonarReadings <<", ";
    }
    sonarReadings <<"\n";
    sonarReadings.close();
    //moveServo(220);

    //wait(3000);

    g_servoCtrl.setPos(130);

    wait(1000);

}



