

#include <sys/time.h>
#include <math.h>
#include "conf.h"

typedef struct{
double x,y,theta;
}pose;

class Pose{


    public:
        pose robotPose;
        timeval starttime;
        timeval endtime;
        double velocity;
        double distance;

        Pose(double x,double y, double theta );
        ~Pose();
        void setTimestamp();
        void updatePosition(double distance);
        void updateAngle(double angle);
        pose getPose();
        void setPose(double x,double y, double theta );


};
