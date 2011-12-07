#ifndef POSE_H
#define	POSE_H

#include <sys/time.h>
#include <math.h>
#include "conf.h"


typedef struct{
int direction;
unsigned long time;
}DirTime;

typedef struct{
double x,y,theta;
double ret_value;
}RobotPose;



class Pose{
    public:
        RobotPose robotPose;
        timeval starttime;
        timeval endtime;
        double distance;

        Pose(double x,double y, double theta );
        ~Pose();
        void setTimestamp();
        void updatePosition();
        void updateAngle();
        DirTime shiftToGoal(SITE id_site);
        RobotPose getPose();
        void setPose(double x,double y, double theta );


    private:
        double angle,goal_x,goal_y,diff_x,diff_y,diff_ang;
        DirTime ret_val;
};
#endif
