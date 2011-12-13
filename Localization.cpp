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
            pose.setPose(142,220,110*M_PI/180);
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
            pose.setPose(0,0,-M_PI/2);
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

    if (siteIndex == SITE_3)
    {
        //directionTime.direction = TURNING_RIGHT;
    }

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

    if (siteIndex == SITE_3)
    {
        wait(4000);
    }
    else
    {
        wait(3000);
    }

    stop();
    sonarScan();
    moveForward();
}


bool scanSite(int sensorIndex);



bool irScanSite(void)
{
    return scanSite(INDEX_SENSOR_IR_TOP);
}

#define VALUE_DIFF_SONAR_EDGE_DIFF      30.0
#define VALUE_DIFF_IR_EDGE_DIFF         50.0

#define VALUE_DIV_SONAR_SURFACE_DIV     5.0
#define VALUE_DIV_IR_SURFACE_DIV        15.0

#define VALUE_LENGTH_SITE_LENGTH_MIN    60.0
#define VALUE_LENGTH_SITE_LENGTH_MAX    160.0

#define VALUE_THRESHOLD_SONAR_GAP_SIZE  2
#define VALUE_THRESHOLD_IR_GAP_SIZE     15

/** \brief Convert the value from IR sensor to centimetres
 *
 * \param sensorValue double the    value from IR sensor
 * \return double                   centimetres
 *
 */
double cvrtIrValue2Cm(double sensorValue)
{
    return (4800.0/(sensorValue - 20.0));
}

/** \brief Convert the servo position to radian
 *
 * \param servoPos double   the servo position
 * \return double           the corresponding radian
 *
 */
double cvrtServoPos2Rad(double servoPos)
{
    return servoPos*(M_PI/220);
}

/** \brief Convert the readings from sensor to site measurements
 *
 * \param measurement SITE_MEASUREMENT& the measurements
 * \param leftPos     const double      the position of the servo when detect the left edge
 * \param centrePos   const double      the position of the servo when detect the centre
 * \param rightPos    const double      the position of the servo when detect the right edge
 * \param leftValue   const double      the value from the sensor when detect the left edge
 * \param centreValue const double      the value from the sensor when detect the centre
 * \param rightValue  const double      the value from the sensor when detect the right edge
 * \return void
 *
 */
void cvrtReadings2SiteMeasurement(SITE_MEASUREMENT &measurement,
                                  const double leftPos,
                                  const double centrePos,
                                  const double rightPos,
                                  const double leftValue,
                                  const double centreValue,
                                  const double rightValue)
{
    measurement.radLeft  = cvrtServoPos2Rad(centrePos - leftPos);
    measurement.radRight = cvrtServoPos2Rad(rightPos - centrePos);
    measurement.rngLeft  = leftValue;
    measurement.rngMid   = centreValue;
    measurement.rngRight = rightValue;

    #if DEBUG_MODE_SCAN
    cout << "leftPos: " << leftPos << " centrePos: " << centrePos << " rightPos: " << rightPos << endl;
    cout << "radLeft : " << measurement.radLeft << endl;
    cout << "radRight: " << measurement.radRight << endl;
    cout << "leftValue  : " << leftValue << endl;
    cout << "centreValue: " << centreValue << endl;
    cout << "rightValue : " << rightValue << endl;
    #endif
}

bool bIsSite(const double leftPos,
             const double centrePos,
             const double rightPos,
             const double leftValue,
             const double centreValue,
             const double rightValue)
{
    #if DEBUG_MODE_SCAN
    cout << "<==== TEST SITE ====>" << endl;
    #endif

    SITE_MEASUREMENT measurements;
    Feature feature;

    cvrtReadings2SiteMeasurement(measurements, leftPos, centrePos, rightPos, leftValue, centreValue, rightValue);

    calFeatures(feature, measurements);

    double totalLength = feature.lenLeft + feature.lenRight;

    #if DEBUG_MODE_SCAN
    cout << "site lenLeft: " << feature.lenLeft << " lenRight: " << feature.lenRight << " radian: " << feature.rad << endl;
    #endif

    // if the value is within the site range
    if (   (totalLength > VALUE_LENGTH_SITE_LENGTH_MIN)
        && (totalLength < VALUE_LENGTH_SITE_LENGTH_MAX))
    {
        return true;
    }

    return false;
}

Feature g_siteFeature;
SITE_MEASUREMENT g_scanMeasurements;
double g_gapPosition;

bool scanSite(int sensorIndex)
{
    double scanStep = 1.0;  /**< the step for servo to move each time */
    double curPos   = VALUE_SERVO_POS_MIN;  /**< current position of the servo motor */

    double curValue = 0.0;      /**< the current value from the sensor */
    double preValue = 0.0;      /**< the previous value from the sensor */

    double leftPos  = 0.0;      /**< the position of the left edge */
    double gapPos   = 0.0;      /**< the position of the gap */
    double rightPos = 0.0;      /**< the position of the right edge */

    double leftValue   = 0.0;   /**< the value from the left edge */
    double centreValue = 0.0;   /**< the value from the centre */
    double rightValue  = 0.0;   /**< the value from the right edge */

    double closeValue  = 0.0;   /**< after detect the left edge, set the value as close value */

    bool bFoundLeftEdge  = false;   /**< the left edge is found */
    bool bFoundGap       = false;   /**< the gap is found */
    bool bFoundRightEdge = false;   /**< the right edge if found */

    int  continuityCounter = 0;
    int  totalCounter      = 0;

    double edgeDiff   = 30.0;
    double surfaceDiv = 2.0;

    int gapThreshold  = 5;

    ofstream sonarReadings;
    sonarReadings.open("SensorSO.txt", fstream::app);

    sonarReadings << "--- TEST: ";

    if (INDEX_SENSOR_IR_TOP == sensorIndex)
    {
        sonarReadings << "IR";

        edgeDiff     = VALUE_DIFF_IR_EDGE_DIFF;
        surfaceDiv   = VALUE_DIV_IR_SURFACE_DIV;
        gapThreshold = VALUE_THRESHOLD_IR_GAP_SIZE;
    }
    else
    {
        sonarReadings << "Sonar";

        edgeDiff     = VALUE_DIFF_SONAR_EDGE_DIFF;
        surfaceDiv   = VALUE_DIV_SONAR_SURFACE_DIV;
        gapThreshold = VALUE_THRESHOLD_SONAR_GAP_SIZE;
    }

    sonarReadings << " ---" << endl;

    curValue = GetSensorValue(sensorIndex);

    // prepare the servo, set it to the very left end
    // Use sonar to scan 180

    // there will be total VALUE_SERVO_POS_MAX/scanStep values received

    for (curPos = VALUE_SERVO_POS_MIN; curPos < VALUE_SERVO_POS_MAX; curPos += scanStep)
    {
        // using the waited set position function, the servo will be in the desired position
        g_servoCtrl.setPosWait(curPos);

        // get the value from sonar
        preValue = curValue;
        curValue = GetSensorValue(sensorIndex);

        // if IR SENSOR
        if (INDEX_SENSOR_IR_TOP == sensorIndex)
        {
            #if 1
            // the values smaller than 80 or lager than 500 is not reliable based on the data sheet
            if (   (curValue <= 80.0)
                || (curValue >= 500.0))
            {
                curValue = 200.0;
                // ignore the following processing
                // continue;
            }
            else
            {
                // convert the readings to CM
                curValue = cvrtIrValue2Cm(curValue);
            }
            #endif
        }
        else
        {
            if (   (curValue > 150)
                || (curValue < 3))
            {
                curValue = 500;
            }
        }

        // log the readings
        sonarReadings << curPos << ", " << curValue << endl;
        // cout << curPos << ", " << curValue << endl;

        // we will only scan the site exposed within our 180 degree scanning area
        // we are only interested in the first left edge
        // down trend
        if ((preValue - curValue) > edgeDiff)
        {
            // if the left edge is not found
            if (!bFoundLeftEdge)
            {
                cout << "--left edge" << endl;

                // set left edge as found
                bFoundLeftEdge  = true;
                bFoundGap       = false;
                bFoundRightEdge = false;

                // record the left edge value and position
                leftValue  = curValue;
                leftPos    = curPos;
                closeValue = curValue;

                // reset the total counter
                totalCounter = 0;
            }
            else
            {
                // if both the left and right edges are found
                if (bFoundRightEdge)
                {
                    // but it is too short, which is a gap
                    if (continuityCounter < gapThreshold)
                    {
                        // set the right edge as not found
                        bFoundRightEdge = false;

                        cout << "..gap" << endl;
                        // set the values for gap
                        bFoundGap   = true;
                        gapPos      = curPos;
                        centreValue = curValue;

                        // accumulate the total counter
                        totalCounter += continuityCounter;
                    }
                    else
                    {
                        // if the gap is not found, then set the gap as the mean of the left and right
                        if (!bFoundGap)
                        {
                            gapPos = leftPos + (rightPos - leftPos) * 0.5;
                            centreValue = (leftValue + rightValue) * 0.5;
                        }

                        // test if it is a site already
                        if (bIsSite(leftPos, gapPos, rightPos, leftValue, centreValue, rightValue))
                        {
                            break;
                        }
                        else
                        {
                            cout << "**clean: found another down trend, and the prev site is too short" << endl;
                            // set everything to zero
                            bFoundLeftEdge  = false;
                            bFoundGap       = false;
                            bFoundRightEdge = false;

                            totalCounter    = 0;
                        }
                    }
                }
                // the left edge was found
                else
                {
                    preLeftPos = leftPos;
                }
            }

            // since it is an change of the continuity, set it to 0
            continuityCounter = 0;
        }
        // up trend
        else if ((curValue - preValue) > edgeDiff)
        {
            // only if the left edge is detected
            if (bFoundLeftEdge)
            {
                cout << "++right edge" << endl;
                // set right edge as found
                bFoundRightEdge = true;

                rightValue = preValue;
                rightPos   = curPos - scanStep;

                totalCounter += continuityCounter;

                if (bFoundGap)          // if the gap and right edge are both found
                {
                    if (bIsSite(leftPos, gapPos, rightPos, leftValue, centreValue, rightValue))
                    {
                        break;
                    }
                    else
                    {
                        cout << "**clean: gap, right both found, but site is too short" << endl;
                        // set everything to zero
                        bFoundLeftEdge  = false;
                        bFoundGap       = false;
                        bFoundRightEdge = false;

                        totalCounter    = 0;
                    }
                }
            }

            // since it is an change of the continuity, set it to 0
            continuityCounter = 0;
        }
        else
        {
            // if the difference between previous value and current value is smaller than the threshold
            if (abs(curValue - preValue) < surfaceDiv)
            {
                // increase the continuity counter
                continuityCounter++;

                // if only can find the left and right edge
                if (   (bFoundRightEdge)        // the right edge is found
                    && (continuityCounter > gapThreshold)) // the gap is lager than 10
                {
                    // if the gap is not found, then set the gap as the mean of the left and right
                    if (!bFoundGap)
                    {
                        gapPos = leftPos + (rightPos - leftPos) * 0.5;
                        centreValue = (leftValue + rightValue) * 0.5;
                    }

                    // test if it is a site already
                    if (bIsSite(leftPos, gapPos, rightPos, leftValue, centreValue, rightValue))
                    {
                        break;
                    }
                }
            }
        }
    }

    sonarReadings.close();

    // for sonar scan, it is almost impossible to find the gap, use the median
    if (INDEX_SENSOR_SONAR == sensorIndex)
    {
        gapPos = (leftPos + rightPos) * 0.5;
    }

    g_gapPosition = gapPos;

    cout << "Left : " << bFoundLeftEdge  << " pos: " << leftPos  << " value: " << leftValue << endl;
    cout << "Gap  : " << bFoundGap       << " pos: " << gapPos   << " value: " << centreValue << endl;
    cout << "Right: " << bFoundRightEdge << " pos: " << rightPos << " value: " << rightValue << endl;

    // if the right edge is found
    if (bFoundRightEdge && bIsSite(leftPos, gapPos, rightPos, leftValue, centreValue, rightValue))
    {
        cout << "scanSite - site found." << endl;
        cvrtReadings2SiteMeasurement(g_scanMeasurements, leftPos, gapPos, rightPos, leftValue, centreValue, rightValue);
        calFeatures(g_siteFeature, g_scanMeasurements);

        return true;
    }

    cout << "scanSite - site NOT found." << endl;
    return false;
}

bool bSonarScan(int sensorIndex)
{
    double scanStep = 1.0;  /**< the step for servo to move each time */
    double curPos   = VALUE_SERVO_POS_MIN;  /**< current position of the servo motor */

    double curValue = 0.0;      /**< the current value from the sensor */
    double preValue = 0.0;      /**< the previous value from the sensor */

    double leftPos  = 0.0;      /**< the position of the left edge */
    double gapPos   = 0.0;      /**< the position of the gap */
    double rightPos = 0.0;      /**< the position of the right edge */

    double leftPosPre  = 0.0;   /**< the position of the left edge */
    double gapPosPre   = 0.0;   /**< the position of the gap */
    double rightPosPre = 0.0;   /**< the position of the right edge */

    double leftValue    = 0.0;   /**< the value from the left edge */
    double leftValuePre = 0.0;   /**< the value from the left edge */
    double centreValue  = 0.0;   /**< the value from the centre */
    double rightValue   = 0.0;   /**< the value from the right edge */

    int biggestPlane      = 0;
    int continuityCounter = 0;

    ofstream sonarReadings;
    sonarReadings.open("SensorSO.txt", fstream::app);
    sonarReadings << "--- TEST: Sonar ---";

    curValue = GetSensorValue(sensorIndex);

    // prepare the servo, set it to the very left end
    // Use sonar to scan 180

    // there will be total VALUE_SERVO_POS_MAX/scanStep values received

    for (curPos = VALUE_SERVO_POS_MIN; curPos < VALUE_SERVO_POS_MAX; curPos += scanStep)
    {
        // using the waited set position function, the servo will be in the desired position
        g_servoCtrl.setPosWait(curPos);

        // get the value from sonar
        preValue = curValue;
        curValue = GetSensorValue(sensorIndex);

        if (   (curValue > 150)
            || (curValue < 3))
        {
            curValue = 500;
        }

        // log the readings
        sonarReadings << curPos << ", " << curValue << endl;
        // cout << curPos << ", " << curValue << endl;

        // we will only scan the site exposed within our 180 degree scanning area
        // we are only interested in the first left edge
        // down trend

        // if the difference between previous value and current value is smaller than the threshold
        if (curValue == preValue)
        {
            // increase the continuity counter
            continuityCounter++;
        }
        else
        {
            // if only can find the left and right edge
            if (continuityCounter > biggestPlane)
            {
                //
                leftPos   = leftPosPre;
                leftValue = leftValuePre;

                rightPos   = curPos;
                rightValue = curValue;

                biggestPlane = continuityCounter;
            }

            leftPosPre   = curPos;
            leftValuePre = curValue;

            continuityCounter = 0;
        }
    }

    sonarReadings.close();

    // for sonar scan, it is almost impossible to find the gap, use the median
    gapPos = (leftPos + rightPos) * 0.5;

    g_gapPosition = gapPos;

    cout << "Left : pos: " << leftPos  << " value: " << leftValue << endl;
    cout << "Gap  : pos: " << gapPos   << " value: " << centreValue << endl;
    cout << "Right: pos: " << rightPos << " value: " << rightValue << endl;

    return true;
}

bool sonarScanSite(void)
{
    return bSonarScan(INDEX_SENSOR_SONAR);
}

void Localization::sonarScan(void)
{
    cout<<"Entered Sonar Scanning Mode..."<<endl;
    //robotStatus = STATUS_ROBOT_SONAR_SCANNING;

    int firstServoMinPosition = -1;
    int lastServoMinPosition  = -1;
    int minSonarValue = 1000;

    double currentPosition;
    double sonarValue;
    //The distance the servo has moved from the center position
    double servoOffset = 0.0;

    //The number of degrees that the robot needs to turn
    double radianTurn=0.0;
    unsigned long turnMilisecs=0;
    //Find the nearest obstacle

    g_servoCtrl.setPos(0);
    wait(2000);

    //Get the current servo position
    currentPosition = g_servoCtrl.getPos();

    double sonarValuePre = g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR);

    double sonarValueTest = 0.0;

    int leftPosition = 0;
    bool bLeftFound = false;
    int rightPosition = 0;
    bool bRightFound = false;

    int rightEdgeCounter = 0;
    int leftEdgeCounter  = 0;

    int finalPosition = 125;

    for (int i = 5; i < 220; i = i+5)
    {
        g_servoCtrl.setPos(i);
        //Get the current servo position
        currentPosition = g_servoCtrl.getPos();

        //Set the current sonar value
        sonarValue = g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR);
        //CPhidgetAdvancedServo_getPosition (servo, 0, &currentPosition);

        // first the left edge
        if ((sonarValuePre - sonarValue) > 25)
        {
            if (leftEdgeCounter == 0)
            {
                sonarValueTest = sonarValuePre;
            }

            leftEdgeCounter++;

            leftPosition = currentPosition;

            // continued for 3 times, consider it as the left edge
            if (leftEdgeCounter >= 1)
            {
                // recall the values of 3 steps ago
                leftPosition -= leftEdgeCounter*5;
                cout << "sonarScan - found left edge: " << leftPosition <<  endl;
                bLeftFound = true;
            }
        }

        // only if the left edge is found
        if (bLeftFound)
        {
            // find the right edge
            if ((sonarValue - sonarValuePre) > 25)
            {
                // set right position
                rightPosition = currentPosition;

                rightEdgeCounter++;

                if (rightEdgeCounter >= 1)
                {
                        // recall the values of 3 steps ago
                        rightPosition -= rightEdgeCounter*5;
                        cout << "sonarScan - found right edge: " << leftPosition <<  endl;
                        bRightFound = true;
                        break;
                }

            }
        }

        sonarValuePre = sonarValue;

        wait(200);

#if DEBUG_MODE_SONAR_SCAN
        cout<<"Sonar Position: "<<currentPosition<<endl;
        cout<<"Sonar Value: " <<g_sensorCtrl.getSensorValue(INDEX_SENSOR_SONAR)<<endl;
#endif
    }

    if (bRightFound)
    {
        finalPosition = leftPosition + ((rightPosition - leftPosition) / 2);
    }
    else
    {
        if (bLeftFound)
        {
            finalPosition = leftPosition + 60;
        }
        else
        {
            cout << "sonarScan - found nothing, set a fixed turning position: 50" << endl;
            finalPosition = 20;
        }
    }

    cout << "sonarScan - the final position: " << finalPosition <<  endl;

#if DEBUG_MODE_SONAR_SCAN
    cout<<"Initial Min Position: "<<firstServoMinPosition<<endl;
    cout<<"Final Min Position  : "<<lastServoMinPosition<<endl;
#endif

    // set it to the centre
    g_servoCtrl.setPos(VALUE_SERVO_POS_MID);

    servoOffset = finalPosition - VALUE_SERVO_POS_MID;


    //0.0142 is the number of degrees that the servo turns after each step as the range
    //is from 0 to 220. Dividing 180 degrees by the number of steps yields the number
    //of degrees per servo step
    radianTurn = servoOffset*0.0142;

    cout << "sonarScan - the turning offset" << servoOffset << endl;
    cout << "sonarScan - the radian need to turn" << radianTurn << endl;

    turnMilisecs = abs(radianTurn/ang_velocity);

    if(servoOffset < 0)
    {
        //How long the car must turn clockwise (in milisecs) to reach the current servo position.

        turnLeft();
    }
    else
    {
        //Note the turnMilisecs are unequal as the car requires slightly more time to turn clockwise
        turnRight();
    }

    cout<<"Number of radian is: "<<radianTurn<<" , and number of milisecs is: "<<turnMilisecs<<endl;

    wait(turnMilisecs);

    //Move towards the new obstacle
    //ActMoveForward(0);

    // need to set some value
    sonarMeasurement = minSonarValue;

    sonarMeasurementPosition = finalPosition;

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



