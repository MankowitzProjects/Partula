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

Localization::Localization()
{

}

Localization::~Localization()
{

}

//Identifies the current site and updates the robots position
void Localization::identifySite(FREQUENCY frequency)
{
    switch(frequency)
    {
    case FREQUENCY_HALF:
    {
        pose.setPose(1,1,2);
        break;
    }
    case FREQUENCY_1:
    {
        pose.setPose(1,2,2);
        break;
    }
    case FREQUENCY_2:
    {
        pose.setPose(1,3,2);
        break;
    }
    case FREQUENCY_4:
    {
        pose.setPose(1,4,2);
        break;
    }
    case FREQUENCY_6:
    {
        pose.setPose(1,5,2);
        break;
    }
    case FREQUENCY_8:
    {
        pose.setPose(1,6,2);
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
void Localization::EstimateResourceSiteLocations()
{



}

//Take a measurement of the nearby obstacles
void Localization::takeMeasurement()
{

    int sonarStatus = 0;
    pthread_t sonarThread;
    pthread_create(&sonarThread, NULL, sonarScan,(void*)&sonarStatus);


}

void Localization::updateParticles()
{



}

void* Localization::sonarScan(void* param)
{
    cout<<"Entered Sonar Scanning Mode..."<<endl;
    robotStatus=STATUS_ROBOT_EXPLORING;

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
      g_servoCtrl.getPos();

      //Set the current sonar value
      sonarValue = g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR);
      //CPhidgetAdvancedServo_getPosition (servo, 0, &currentPosition);
      if(sonarValue<minSonarValue){


            minSonarValue = sonarValue;

            cout<<"Sonar Value: "<<currentPosition;
            minSonarPosition = currentPosition;
        }
        wait(200);

    }

    g_servoCtrl.setPos(130);

    servoOffset = 120 - minSonarPosition;

    cout<<"Servo offset: "<<servoOffset;
    //0.81 is the number of degrees that the servo turns after each step as the range
    //is from 0 to 220. Dividing 180 degrees by the number of steps yields the number
    //of degrees per servo step
    degreeTurn = servoOffset*0.81;


    if(servoOffset>0)
    {
        cout<<"servo offset greater than 0"<<endl;
        //How long the car must turn clockwise (in milisecs) to reach the current servo position.
        turnMilisecs = degreeTurn*16;

        cout<<"Number of degrees is "<<degreeTurn<<"and number of milisecs is "<<turnMilisecs<<endl;
        ActTurnLeft(turnMilisecs);

    }
    else if(servoOffset<0)
    {
        cout<<"servo offset less than 0"<<endl;


        //Note the turnMilisecs are unequal as the car requires slightly more time to turn clockwise
        turnMilisecs = degreeTurn*(-15);

        cout<<"Number of degrees is "<<degreeTurn<<"and number of milisecs is"<<turnMilisecs<<endl;
        ActTurnRight(turnMilisecs);

      }
      //Move towards the new obstacle
      //ActMoveForward(0);
      //moveForward();


    pthread_exit(NULL);

}



