/*
 * File:   Localization.cpp
 * Author: daniel
 *
 * Created on December 3, 2011, 9:43 PM
 */

#include "Localization.h"

Localization g_localization;
extern Pose pose;

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

   // pthread_create(&sonarThread, NULL, sonarScan,(void*)&sonarStatus);

}

void Localization::updateParticles()
{



}

/*void* Localization::sonarScan(void* param)
{
	printf("Entered Sonar Scanning Mode...\n");
	robotStatus=0;

      int servoPosition = -1;
      int minSonarValue = 1000;
      double minSonarPosition = -1.0;
      double currentPosition;

      //The distance the servo has moved from the center position
      double servoOffset = 0.0;

      //The number of degrees that the robot needs to turn
      double degreeTurn=0.0;
      unsigned long turnMilisecs=0;
      //Find the nearest obstacle
	for (int i=0;i<220;i=i+5)
	{
	  moveServo(i);
	  //Get the current servo position
	  CPhidgetAdvancedServo_getPosition (servo, 0, &currentPosition);
	  if(sonarValue<minSonarValue){

	    minSonarValue = sonarValue;

	    printf("\nSonar Value %f\n", currentPosition);
	    minSonarPosition = currentPosition;
	  }
	  wait(200);

 	}

	moveServo(130);

	servoOffset = 120 - minSonarPosition;

	printf("\nServo offset %f\n\n",servoOffset);
	  //0.81 is the number of degrees that the servo turns after each step as the range
	  //is from 0 to 220. Dividing 180 degrees by the number of steps yields the number
	  //of degrees per servo step
	degreeTurn = servoOffset*0.81;


	if(servoOffset>0){
	  printf("\n\nservo offset greater than 0\n\n");
	//How long the car must turn clockwise (in milisecs) to reach the current servo position.
	turnMilisecs = degreeTurn*16;

	printf("\n\nNumber of degrees is %f and number of milisecs is %d\n\n",degreeTurn, turnMilisecs);
	turn_left(navigAcc, navigSpeed, turnMilisecs);

	}
	else
	  if(servoOffset<0)
	  {
	    printf("\n\nservo offset less than 0\n\n");


	    //Note the turnMilisecs are unequal as the car requires slightly more time to turn clockwise
	   turnMilisecs = degreeTurn*(-15);

	   printf("\n\nNumber of degrees is %f and number of milisecs is %d\n\n",degreeTurn, turnMilisecs);
	   turn_right(navigAcc, navigSpeed, turnMilisecs);

	  }
      //Move towards the new obstacle
      go_straight(navigAcc, navigSpeed, 0);

      pthread_exit(NULL);

}*/



