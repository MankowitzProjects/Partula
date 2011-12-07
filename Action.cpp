#include "Action.h"
#include "debug_tools.h"
#include <pthread.h>
#include "conf.h"

extern MotorController g_motorCtrl;
extern EVENT currentEvent;

pthread_t g_thrd_motor;
bool      b_thrd_motor_created = false;

pthread_t g_thrd_scan;

unsigned long g_motor_time;

void moveForward(void);
void moveBackward(void);
void turnLeft(void);
void turnRight(void);
void stop(void);

void *p_thrd_moveForward(void *para);
void *p_thrd_moveBackward(void *para);
void *p_thrd_turnLeft(void *para);
void *p_thrd_turnRight(void *para);
void *p_thrd_hitBumperFront(void *para);
void *p_thrd_hitBumperLeft(void *para);
void *p_thrd_hitBumperRight(void *para);

void _kill_thrd_motor(void);



void __kill_thrd_motor(void)
{
    prt_debug("Action: kill the motor thread\n");

    if (b_thrd_motor_created)
    {
        pthread_cancel(g_thrd_motor);

        b_thrd_motor_created = false;
    }
}

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

    b_thrd_motor_created = true;
}

void ActStop(void)
{
    prt_debug("ActStop\n");

    // kill the thread
    __kill_thrd_motor();

    stop();
}

void ActMoveForward(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActMoveForward: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_moveForward, milisec);
}

void ActMoveBackward(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActMoveBackWard: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_moveBackward, milisec);
}

void ActTurnLeft(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActTurnLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_turnLeft, milisec);
}

void ActTurnRight(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActTurnRight: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_turnRight, milisec);
}

void ActHitBumperFront(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperFront: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperFront, milisec);
}

void ActHitBumperLeft(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperLeft, milisec);
}

void ActHitBumperRight(unsigned long milisec)
{
#if (DEBUG_MODE)
    printf("ActHitBumperLeft: %d\n", (unsigned int)milisec);
#endif
    __start_thrd_motor(p_thrd_hitBumperRight, milisec);
}


void moveForward(void)
{
    //MOVEMENTSTATUS = FORWARD;
    g_motorCtrl.setAcc(VALUE_MOTOR_ACC_NORMAL);
    g_motorCtrl.setVel(VALUE_MOTOR_VEL_NORMAL);
}

void moveBackward(void)
{
    g_motorCtrl.setAcc(-VALUE_MOTOR_ACC_NORMAL);
    g_motorCtrl.setVel(-VALUE_MOTOR_VEL_NORMAL);
}

void turnLeft(void)
{
    g_motorCtrl.setVelLeft(VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccLeft(VALUE_MOTOR_ACC_NORMAL);

    g_motorCtrl.setVelRight(-VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccRight(-VALUE_MOTOR_ACC_NORMAL);
}

void turnRight(void)
{
    g_motorCtrl.setVelLeft(-VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccLeft(-VALUE_MOTOR_ACC_NORMAL);

    g_motorCtrl.setVelRight(VALUE_MOTOR_VEL_NORMAL);
    g_motorCtrl.setAccRight(VALUE_MOTOR_ACC_NORMAL);
}

void stop(void)
{
    prt_debug("MotorStop\n");
    g_motorCtrl.stop();
}

void* p_thrd_moveForward(void *para)
{
    moveForward();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        stop();
    }

    prt_debug("p_thrd_moveForward stopped\n");

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_moveBackward(void *para)
{
    moveBackward();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        stop();
    }

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_turnLeft(void *para)
{
    turnLeft();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        stop();
    }

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_turnRight(void *para)
{
    turnRight();

    if (0 != *((unsigned long *)para))
    {
        wait(*((unsigned long *)para));

        stop();
    }

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_hitBumperFront(void *para)
{
    moveBackward();
    wait(*((unsigned long *)para));
    turnLeft();
    wait(*((unsigned long *)para));
    moveForward();

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_hitBumperLeft(void *para)
{
    moveBackward();
    wait(*((unsigned long *)para));
    turnRight();
    wait(*((unsigned long *)para));
    moveForward();

    pthread_exit(NULL);

    return NULL;
}

void* p_thrd_hitBumperRight(void *para)
{
    moveBackward();
    wait(*((unsigned long *)para));
    turnLeft();
    wait(*((unsigned long *)para));
    moveForward();

    pthread_exit(NULL);

    return NULL;
}


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

void frequencyMovement(FREQUENCY frequency)
{

    switch(frequency)
    {

    case FREQUENCY_HALF:
        ActMoveBackward( 1000);
        ActTurnLeft( 5600);
        stop();
        break;
    case FREQUENCY_1:
        ActMoveBackward( 1000);
        ActTurnRight( 5600);
        stop();
        break;
    case FREQUENCY_2:
        ActMoveBackward( 1000);
        stop();
        ActMoveBackward( 1000);
        stop();
        break;
    case FREQUENCY_4:
        ActMoveBackward( 1000);
        ActTurnLeft( 2600);
        stop();
        break;
    case FREQUENCY_6:
        ActMoveBackward( 2000);
        ActTurnRight( 2600);
        stop();
        break;
    case FREQUENCY_8:
        ActMoveBackward( 2000);
        stop();
        ActMoveForward( 1000);
        stop();
        break;
    default:

        break;

    }

}

