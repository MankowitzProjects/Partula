#ifndef POSE_H
#define	POSE_H

#include <sys/time.h>
#include <math.h>
#include "conf.h"
#include "math_ext.h"
#include "Site.h"

//Direction and time given to localization
typedef struct
{
    int direction;
    unsigned long time;
} DirTime;

//Robot's pose

typedef struct{
double x,y,theta;
}RobotPose;

extern double velocity;
extern double ang_velocity;

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
        double angle,goal_x,goal_y,diff_ang;
        DirTime ret_val;
};

#endif
