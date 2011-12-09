#include "Pose.h"

MOVEMENT_STATUS g_movement;
extern Site sites[NUM_SITE];

extern Pose pose;
double velocity;
double ang_velocity;

Pose::Pose(double x,double y, double theta)
{
    robotPose.x=x;
    robotPose.y=y;
    robotPose.theta=theta;
    velocity = 15.5;
    ang_velocity = 0.775;
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
    gettimeofday(&endtime,NULL);
    distance=velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
    if ((MOVING_FORWARD==g_movement)||(MOVING_BACKWARD==g_movement)){
    robotPose.x+=g_movement*distance*cos(robotPose.theta);
    robotPose.y+=g_movement*distance*sin(robotPose.theta);
    }
    else if (TURNING_LEFT==g_movement){
        robotPose.theta+=ang_velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
        robotPose.theta=normRad(robotPose.theta);
    }
    else if(TURNING_RIGHT==g_movement){
        robotPose.theta-=ang_velocity*(((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec)))/1000000.0;
        robotPose.theta=normRad(robotPose.theta);
    }
    else if(STOPPED==g_movement){

    }

    starttime.tv_sec=endtime.tv_sec;
    starttime.tv_usec=endtime.tv_usec;
    cout<<"New Position: (x,y,angle): "<<"("<<robotPose.x<<", "<<robotPose.y<<", "<<robotPose.theta<<")"<<endl;

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

    cout<<"Angle: "<<angle<<endl;
    angle=normRad(angle);
    cout<<"NormRad: "<<angle<<endl;

    ret_val.time=abs((angle/ang_velocity)*1000);
    if (angle>0){
        ret_val.direction=TURNING_RIGHT;
    }
    else{
        ret_val.direction=TURNING_LEFT;
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

    cout<<"New Position: (x,y): "<<"("<<robotPose.x<<", "<<robotPose.y<<")"<<endl;
}

int Pose::distanceToResourceSite(SITE site){

        double x_site = sites[site].readyPoint.x;
        double y_site = sites[site].readyPoint.y;

        double distanceToSite = sqrt(pow((robotPose.x - x_site),2) + pow((robotPose.y - y_site),2));

        cout<<"Distance to site: "<<distanceToSite<<endl;
        return distanceToTime(distanceToSite);
}

double Pose::distanceToTime(double distance){

    double time = distance/velocity*1000;

    return time;

}



