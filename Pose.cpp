#include "Pose.h"

    Pose::Pose(double x,double y, double theta)
    {
        robotPose.x=x;
        robotPose.y=y;
        robotPose.theta=theta;
    }

    Pose::~Pose()
    {
        //dtor
    }

     void Pose::setTimestamp(){

        gettimeofday(&starttime,NULL);

        }

        void Pose::updatePosition(double distance){
        gettimeofday(&endtime,NULL);
        distance=velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec));
        robotPose.x+=distance*cos(robotPose.theta);
        robotPose.y+=distance*sin(robotPose.theta);
        }

        void Pose::updateAngle(double angle){
        robotPose.theta=angle;
        }

        pose Pose::getPose(){
        return robotPose;
        }

        void Pose::setPose(double x,double y, double theta ){
        robotPose.x=x;
        robotPose.y=y;
        robotPose.theta=theta;
        }


