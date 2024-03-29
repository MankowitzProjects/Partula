#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "MotorController.h"
#include "Pose.h"

void ActMoveForward(unsigned long milisec);
void ActMoveBackward(unsigned long milisec);
void ActTurnLeft(unsigned long milisec);
void ActTurnRight(unsigned long milisec);
void ActStop(void);

void ActHitBumperFront(unsigned long milisec);
void ActHitBumperLeft(unsigned long milisec);
void ActHitBumperRight(unsigned long milisec);


void hitBumper(long unsigned waitTime);

void frequencyMovement(FREQUENCY frequency);

void moveForward(void);
void moveBackward(void);
void turnLeft(void);
void turnRight(void);
void stop(void);

#endif // ACTION_H_INCLUDED
