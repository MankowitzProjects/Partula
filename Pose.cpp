#include "Pose.h"

    Pose pose(0,0,0);

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

        void Pose::updatePosition(){
        gettimeofday(&endtime,NULL);
        distance=velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec));
        robotPose.x+=distance*cos(robotPose.theta);
        robotPose.y+=distance*sin(robotPose.theta);
        }

        void Pose::updateAngle(){
        gettimeofday(&endtime,NULL);
        robotPose.theta=ang_velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec));
        if (robotPose.theta>2*PI){
        robotPose.theta-=2*PI;
        }
        else if(robotPose.theta<0){
        robotPose.theta+=2*PI;
        }
        }

        void Pose::shiftToGoal(SITE id_site){
        //get goal coordinates
        goal_y=sites[id_site].readypoint.y;
        goal_x=sites[id_site].readypoint.x;
        //relative position of goal to robot
        diff_y=goal_y-robotPose.y;
        diff_x=goal_x-robotPose.x;
        //find angle
        angle=atan(diff_y/diff_x);

        if (diff_x>0&&diff_y>0){
            //calculate how many rads turn should be
            diff_ang=angle-robotPose.theta;
            //dtermine direction
            if(diff_ang>0){
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=2;
            }
            else{
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=1;
            }
            return ret_val;
        }
        else if(diff_x<0&&diff_y<0){
            angle=PI+angle;
            diff_ang=angle-robotPose.theta;
            if(diff_ang>0){
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=2;
            }
            else{
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=1;
            }
            return ret_val;
        }
        else if(diff_x>0&&diff_y<0){
            angle=2*PI-angle;
            diff_ang=angle-robotPose.theta;
            if(diff_ang>0){
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=2;
            }
            else{
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=1;
            }
            return ret_val;
        }
        else if(diff_x<0&&diff_y>0){
            angle=PI-angle;
            diff_ang=angle-robotPose.theta;
            if(diff_ang>0){
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=2;
            }
            else{
                ret_val.time=(diff_ang/ang_velocity)*1000;
                ret_val.direction=1;
            }
            return ret_val;
        }
        }
        robotPose Pose::getPose(){
        return robotPose;
        }

        void Pose::setPose(double x,double y, double robotPose.theta ){
        robotPose.x=x;
        robotPose.y=y;
        robotPose.robotPose.theta=theta;
        }



