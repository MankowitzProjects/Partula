#include "Action.h"
#include "debug_tools.h"
#include <pthread.h>
#include "conf.h"

extern MotorController g_motorCtrl;
extern EVENT currentEvent;
extern Pose pose;
extern MOVEMENT_STATUS g_movement;


pthread_t g_thrd_motor;
bool      b_thrd_motor_created = false;

pthread_t g_thrd_scan;

unsigned long g_motor_time;


void *p_thrd_moveForward(void *para);
void *p_thrd_moveBackward(void *para);
void *p_thrd_turnLeft(void *para);
void *p_thrd_turnRight(void *para);
void *p_thrd_hitBumperFront(void *para);
void *p_thrd_hitBumperLeft(void *para);
void *p_thrd_hitBumperRight(void *para);

void _kill_thrd_motor(void);


//The thread that kills a thread that is currently running if another thread has started
void __kill_thrd_motor(void)
{
    prt_debug("Action: kill the motor thread\n");

    if (b_thrd_motor_created)
    {
        pthread_cancel(g_thrd_motor);

        b_thrd_motor_created = false;
    }
}

//The function that initialises a new thread
void __start_thrd_motor(void *func(void *), unsigned long milisec)
{
    int rc = 0;

    __kill_thrd_motor();

    g_motor_time = milisec;

    rc = pthread_create(&g_thrd_motor, NULL, func, (void*)&g_motor_time);
    if (rc)
    {
        printf("** ERROR: __start_thrd_motor, code is %d\n", rc);
    }
    else
    {
        b_thrd_motor_created = true;
    }
}
//The function that starts the motor stop thread
void ActStop(void)
{
    prt_debug("ActStop\n");

    // kill the thread
    __kill_thrd_motor();

    stop();
}
//The function that starts the motor move forward thread
void ActMoveForward(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActMoveForward: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_moveForward, milisec);
}
//The function that starts the motor move backward thread
void ActMoveBackward(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActMoveBackWard: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_moveBackward, milisec);
}
//The function that starts the  motor turn left thread
void ActTurnLeft(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActTurnLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_turnLeft, milisec);
}
//The function that starts the motor turn right thread
void ActTurnRight(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActTurnRight: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_turnRight, milisec);
}
//The function that starts the motor hit bumper fron thread
void ActHitBumperFront(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperFront: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperFront, milisec);
}
//The function that starts the motor hit bumper left thread
void ActHitBumperLeft(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperLeft, milisec);
}
//The function that starts the motor hit bumper right thread
void ActHitBumperRight(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperRight, milisec);
}
//Set the motors to move forward
void moveForward(void)
{
    g_movement = MOVING_FORWARD;
    g_motorCtrl.setAcc(VALUE_MOTOR_ACC_NORMAL);
    g_motorCtrl.setVel(VALUE_MOTOR_VEL_NORMAL);
}
//Set the motors to move backward
void moveBackward(void)
{
    g_movement = MOVING_BACKWARD;
    g_motorCtrl.setAcc(-VALUE_MOTOR_ACC_NORMAL);
    g_motorCtrl.setVel(-VALUE_MOTOR_VEL_NORMAL);
}
//Set the motors to turn left
void turnLeft(void)
{
    g_movement = TURNING_LEFT;
    g_motorCtrl.setVelLeft(-VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccLeft(-VALUE_MOTOR_ACC_NORMAL);

    g_motorCtrl.setVelRight(VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccRight(VALUE_MOTOR_ACC_NORMAL);
}
//Set the motors to turn right
void turnRight(void)
{
    g_movement = TURNING_RIGHT;
    g_motorCtrl.setVelLeft(VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccLeft(VALUE_MOTOR_ACC_NORMAL);

    g_motorCtrl.setVelRight(-VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccRight(-VALUE_MOTOR_ACC_NORMAL);
}
//Set the motors to stop
void stop(void)
{
    g_movement = STOPPED;
    prt_debug("MotorStop\n");
    g_motorCtrl.stop();
}
//Move forward thread
void* p_thrd_moveForward(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
    pose.setTimestamp();

    //Perform the movement
    moveForward();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));
        //Update the position based on the time
       

        stop();
    }

    prt_debug("p_thrd_moveForward stopped\n");

    pthread_exit(NULL);

    return NULL;
}
//Move backward thread
void* p_thrd_moveBackward(void *para)
{
     //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
    pose.setTimestamp();
    moveBackward();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        //Update the position based on the time
       
        stop();
    }

    pthread_exit(NULL);

    return NULL;
}
//turn left thread
void* p_thrd_turnLeft(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
    pose.setTimestamp();
    turnLeft();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));
        //Update the angle based on the time
        

        stop();
    }

    pthread_exit(NULL);

    return NULL;
}
//turn right thread
void* p_thrd_turnRight(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
    pose.setTimestamp();
    turnRight();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        //Update the angle based on the time
        
        stop();
    }

    pthread_exit(NULL);

    return NULL;
}
//hit bumper front thread
void* p_thrd_hitBumperFront(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);

   
    

    pose.setTimestamp();
    moveBackward();
    wait(*((unsigned long *)para));
   

    pose.setTimestamp();
    turnRight();
    wait(*((unsigned long *)para));
    

    pose.setTimestamp();
    moveForward();

    pthread_exit(NULL);

    return NULL;
}
//hit bumper left thread
void* p_thrd_hitBumperLeft(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
   
    cout<<"Hit left bumper. Turning right"<<endl;

    pose.setTimestamp();
    moveBackward();
    wait(*((unsigned long *)para));
   

    pose.setTimestamp();
    turnRight();
    wait(*((unsigned long *)para));
    

    pose.setTimestamp();
    moveForward();

    pthread_exit(NULL);

    return NULL;
}
//hit bumper right thread
void* p_thrd_hitBumperRight(void *para)
{
    //Set the timestamp for updating the position
    double time = *((unsigned long *)para);
   
    
    cout<<"Hit Right bumper. Turning Left"<<endl;
    pose.setTimestamp();
    moveBackward();
    wait(*((unsigned long *)para));
   

    pose.setTimestamp();
    turnLeft();
    wait(*((unsigned long *)para));
    

    pose.setTimestamp();
    moveForward();

    pthread_exit(NULL);

    return NULL;
}

//The state machine for hit bumper events
void hitBumper(long unsigned waitTime)
{
    switch(currentEvent)
    {
    case EVENT_HIT_FRONT:
        ActHitBumperFront(waitTime);
        break;
    case EVENT_HIT_FRONT_LEFT:
        ActHitBumperLeft(waitTime);
        break;
    case EVENT_HIT_FRONT_RIGHT:
        ActHitBumperRight(waitTime);
        break;
    default:

        break;
    }
}
//The state machine for frequency coordinated movements
void frequencyMovement(FREQUENCY frequency)
{
    pose.setTimestamp();
    switch(frequency)
    {

    case FREQUENCY_HALF:
    {
        moveBackward();
        wait(2000);

        turnLeft( );
        wait(7600);
        stop();
        wait(500);
        break;
    }
    case FREQUENCY_1:
    {
        pose.setTimestamp();
        moveBackward();
        wait(2000);

        pose.setTimestamp();
        turnRight();
        wait(7600);

        stop();
        wait(500);
        break;
    }
    case FREQUENCY_2:
    {

        moveBackward();
        wait(2000);

        stop();
        wait(1000);

        pose.setTimestamp();
        moveBackward();
        wait(2000);

        stop();
        wait(500);
        break;
    }
    case FREQUENCY_4:
    {

        moveBackward();
        wait(2000);

        pose.setTimestamp();
        turnLeft( );
        wait(3800);

        stop();
        wait(500);
        break;
    }
    case FREQUENCY_6:
    {

        moveBackward( );
        wait(2000);

        turnRight();
        wait(3800);


        stop();
        wait(500);
        break;
    }
    case FREQUENCY_8:
    {

        moveBackward();
        wait(2000);
    

        stop();
        wait(1000);

        moveForward();
        wait(1000);
    

        stop();
        wait(500);
        break;
    }
    default:
    {
        break;
    }

    }

}

