#ifndef POSE_H
#define	POSE_H

#include <sys/time.h>
#include <math.h>
#include "conf.h"

typedef struct
{
    double x,y,theta;
} RobotPose;

class Pose
{

public:
    RobotPose robotPose;
    timeval starttime;
    timeval endtime;
    double distance;
    double velocity;

    Pose(double x,double y, double theta );
    ~Pose();
    void setTimestamp();
    void updatePosition();
    void updateAngle(double angle);
    RobotPose getPose();
    void setPose(double x,double y, double theta );


};
#endif
