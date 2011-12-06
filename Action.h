#ifndef ACTION_H_INCLUDED
#define ACTION_H_INCLUDED

#include "MotorController.h"



void ActMoveForward(unsigned long milisec);
void ActMoveBackward(unsigned long milisec);
void ActTurnLeft(unsigned long milisec);
void ActTurnRight(unsigned long milisec);
void ActStop(void);

void hitBumper(long unsigned waitTime);

void frequencyMovement(FREQUENCY frequency);




#endif // ACTION_H_INCLUDED
