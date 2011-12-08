#include "Pose.h"

MOVEMENT_STATUS g_movement;
extern Site sites[NUM_SITE];

Pose pose(0,0,0);
double velocity;
double ang_velocity;

Pose::Pose(double x,double y, double theta)
{
    robotPose.x=x;
    robotPose.y=y;
    robotPose.theta=theta;
    velocity = 25;
    ang_velocity = 0.8;
}

Pose::~Pose()
{
    //dtor
}


void Pose::setTimestamp()
{

    gettimeofday(&starttime,NULL);

}

void Pose::updatePosition()
{
    gettimeofday(&endtime,NULL);
    distance=(velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000;
    robotPose.x+=g_movement*distance*cos(robotPose.theta);
    robotPose.y+=g_movement*distance*sin(robotPose.theta);
    cout<<"New Position: (x,y): "<<"("<<robotPose.x<<", "<<robotPose.y<<")"<<endl;
}

void Pose::updateAngle(){
    gettimeofday(&endtime,NULL);
    if (TURNING_LEFT==g_movement){
        robotPose.theta+=(ang_velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000;
    }
    else if(TURNING_RIGHT==g_movement){
        robotPose.theta-=(ang_velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000;
    }
    robotPose.theta=normRad(robotPose.theta);
   cout<<"New Angle: (theta): "<<"("<<robotPose.theta<<")"<<endl;

}

DirTime Pose::shiftToGoal(SITE id_site)
{
    //get goal coordinates

    goal_y=sites[id_site].readyPoint.y;

    goal_x=sites[id_site].readyPoint.x;
    
    goal_y=0;
    goal_x =0;
    //relative position of goal to robot
    //find angle
    angle=atan2(goal_y-robotPose.y,goal_x-robotPose.x);
    angle-=robotPose.theta;
    angle=normRad(angle);
    ret_val.time=robotPose.theta/ang_velocity;
    if (angle>0){
        ret_val.direction=TURNING_LEFT;
    }
    else{
        ret_val.direction=TURNING_RIGHT;
    }
return ret_val;

}
RobotPose Pose::getPose()
{
    return robotPose;
}

void Pose::setPose(double x,double y, double theta )
{
    robotPose.x=x;
    robotPose.y=y;
    robotPose.theta=theta;
}



