#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "MotorController.h"

void ActMoveForward(unsigned long milisec);
void ActMoveBackward(unsigned long milisec);
void ActTurnLeft(unsigned long milisec);
void ActTurnRight(unsigned long milisec);
void ActStop(void);

void hitBumperFront(long unsigned waitTime);
void hitBumperRight(long unsigned waitTime);
void hitBumperLeft(long unsigned waitTime);


void frequency8HzMovement();
void frequency6HzMovement();
void frequency4HzMovement();
void frequency2HzMovement();
void frequency1HzMovement();
void frequencyHalfHzMovement();



#endif // ACTION_H_INCLUDED
