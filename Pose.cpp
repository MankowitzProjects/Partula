#include "Pose.h"

MOVEMENT_STATUS g_movement;
MOVEMENT_STATUS g_prev_movement;
extern Site sites[NUM_SITE];

extern Pose pose;
double velocity;
double ang_velocity;

Pose::Pose(double x,double y, double theta)
{
    robotPose.x=x;//x coordinate of robot
    robotPose.y=y;//y coordinate of robot
    robotPose.theta=theta;//bearing of the robot
    velocity = 15.5;//linear velocity of the robot according to least squares estimation
    ang_velocity = 0.8253;//average angular velocity of the robot according to least squares estimation
}

Pose::~Pose()
{
    //dtor
}


void Pose::setTimestamp()
{

    gettimeofday(&starttime,NULL);

}

void Pose::updateOdometry()
{
    gettimeofday(&endtime,NULL);//get a timestamp that corresponds to the end of the previous action.
    //calculate distance travelled based on linear velocity and the duration of the previous movement
    distance=velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
    //depending on previous movement update odometry.
    if (MOVING_FORWARD==g_prev_movement){
    robotPose.x+=distance*cos(robotPose.theta);
    robotPose.y+=distance*sin(robotPose.theta);
    }
    else if(MOVING_BACKWARD==g_prev_movement){
    robotPose.x+=distance*cos(robotPose.theta+M_PI);
    robotPose.y+=distance*sin(robotPose.theta+M_PI);
    }
    else if (TURNING_LEFT==g_prev_movement){
        robotPose.theta+=ang_velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
        robotPose.theta=normRad(robotPose.theta);
      }
    else if(TURNING_RIGHT==g_prev_movement){
        robotPose.theta-=ang_velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
        robotPose.theta=normRad(robotPose.theta);
    }
    else if(STOPPED==g_prev_movement){
    }
    /*set the start time of the new movement and set the previous movement to current movement for the next call of the function*/
    g_prev_movement=g_movement;
    starttime.tv_sec=endtime.tv_sec;
    starttime.tv_usec=endtime.tv_usec;
    cout<<"New Position: (x,y,angle): "<<"("<<robotPose.x<<", "<<robotPose.y<<", "<<robotPose.theta<<")"<<endl;

}

DirTime Pose::shiftToGoal(SITE id_site)
{
    //get coordinates of the next goal

    goal_y=0;
    goal_x =0;

    goal_y=sites[id_site].readyPoint.y;

    goal_x=sites[id_site].readyPoint.x;


    //find the angle of the line that connects the estimated robot position and the target point of next site.
    angle=atan2(goal_y-robotPose.y,goal_x-robotPose.x);
    /*
    cout<<"goal x: "<<goal_x<<endl;
    cout<<"goal y: "<<goal_y<<endl;
    cout<<"Pose x: "<<robotPose.x<<endl;
    cout<<"Pose y: "<<robotPose.y<<endl;
    */

    //cout<<"After arctan"<<angle<<endl;
    angle-=robotPose.theta;//subtract the current bearing of the robot from the angle to find how much to turn

    //cout<<"Angle: "<<angle<<endl;
    angle=normRad(angle);//normalize so that the angle we find is between minus pi and pi
    //cout<<"NormRad: "<<angle<<endl;

    ret_val.time=abs((angle/ang_velocity)*1000);//calculate time for turn based on turning angle
    //decide on direction of turn depending on the turning angle sign
    if (angle<0){
        ret_val.direction=TURNING_RIGHT;
    }
    else{
        ret_val.direction=TURNING_LEFT;
    }
return ret_val;//return the direction and time of the turn

}
//returns the pose of the robot in a structure
RobotPose Pose::getPose()
{
    return robotPose;
}
//allows the robot's pose to be set maually whenever a certain position is assumed
void Pose::setPose(double x,double y, double theta )
{
    robotPose.x=x;
    robotPose.y=y;
    robotPose.theta=theta;

    cout<<"New Position: (x,y, theta): "<<"("<<robotPose.x<<", "<<robotPose.y<<","<<robotPose.theta<<")"<<endl;
}
//calculates the distance to the next goal in cm
int Pose::distanceToResourceSite(SITE site){

        double x_site = sites[site].readyPoint.x;
        double y_site = sites[site].readyPoint.y;

        double distanceToSite = sqrt(pow((robotPose.x - x_site),2) + pow((robotPose.y - y_site),2));

        cout<<"Distance to site: "<<distanceToSite<<endl;
        return distanceToTime(distanceToSite);
}
//convertion of distance to time based on linear velocity.
double Pose::distanceToTime(double distance){

    double time = distance/velocity*1000;//convertion from cm to milisecs


    return time;

}



