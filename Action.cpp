#include "Action.h"
#include "debug_tools.h"
#include <pthread.h>

extern MotorController g_motorCtrl;

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

void moveForward(void)
{
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


      void hitBumperFront(long unsigned waitTime){
	      ActMoveBackward(waitTime);
	      ActTurnLeft(waitTime);
	      ActMoveForward(waitTime); 
      }

      void hitBumperRight(long unsigned waitTime){
	      ActMoveBackward(waitTime);
	      ActTurnLeft(waitTime);
	      ActMoveForward(0);
	
      }

      void hitBumperLeft(long unsigned waitTime){
	
	  ActMoveBackward(waitTime);
	  ActTurnRight(waitTime);
	  ActMoveForward(0);
	
      }

      void frequency8HzMovement(){
      
	  ActMoveBackward( 2000);
	  stop();
	  ActMoveForward( 1000);
	  stop();
	
      }
      void frequency6HzMovement(){
      
	  ActMoveBackward( 2000);
	  ActTurnRight( 2600);
	  stop();
	
      }
      void frequency4HzMovement(){
      
	  ActMoveBackward( 1000);
	  ActTurnLeft( 2600);
	  stop();
	
      }
      void frequency2HzMovement(){
      
	  ActMoveBackward( 1000);
	  stop();
	  ActMoveBackward( 1000);
	  stop();
	
      }
      void frequency1HzMovement(){
      
	  ActMoveBackward( 1000);
	  ActTurnRight( 5600);
	  stop();
	
      }
      void frequencyHalfHzMovement(){
      
	  ActMoveBackward( 1000);
	  ActTurnLeft( 5600);
	  stop();
	
      }
      
