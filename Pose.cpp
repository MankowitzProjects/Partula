#include <sys/time.h>

typedef struct{
double x,y,theta;
}pose

class Pose{
    pose robotPose;
    timeval startTime,endtime;
    double velocity,distance;

    public:
        getTimestamp(){
        gettimeofday(&starttime,NULL);
        }
        updatePosition(double distance){
        gettimeofday(&endtime,NULL);
        distance=velocity*((endtime.tv_sec * 1000000) + (endtime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec));
        robotPose.x+=distance*cos(robotPose.theta);
        robotPose.y+=distance*sin(robotPose.theta);
        }
        updateAngle(double angle){
        robotPose.theta=angle;
        }
        getPose(){
        return robotPose;
        }
        setPose(double x,double y, double theta ){
        robotPose.x=x;
        robotPose.y=y;
        robotPose.theta=z;
        }
    Pose::Pose(double x,double y, double theta )
    {
        robotPose.x=x;
        robotPose.y=y;
        robotPose.theta=z;
    }

    Pose::~Pose()
    {
        //dtor
    }
}
