/* 
 * File:   Handle.cpp
 * Author: daniel
 * 
 * Created on December 3, 2011, 9:44 PM
 */

#include "Handle.h"
#include "Action.h"

extern EVENT currentEvent;

Handle::Handle() {

}

Handle::Handle(const Handle& orig) {
}

Handle::~Handle() {
}

//Handles collisions
void Handle::collision(){

    if(currentEvent==EVENT_HIT_FRONT){
	  
	  hitBumperFront(1000);

      
    }else if(currentEvent==EVENT_HIT_FRONT_LEFT){
      
    
	  hitBumperLeft(1000);
      
    }else if(currentEvent==EVENT_HIT_FRONT_RIGHT){
      
	  hitBumperRight(1000);
    }

}


void Handle::docking(){


}

void Handle::localization(){
    
}

/*void* Handle::frequency(void* param){
 
      Action action;
      int up,down,counter;

     // while(lightFound==false){};
      //lightFound=false;
      moveServo(130);
      
      printf(" Check frequency\n");    
      check_frequency=1;
      gettimeofday(&starttime,NULL);
      
      up=0;
      timeval currenttime;
      timeval strttime; 
      gettimeofday(&currenttime,NULL);
      
      while (( ((currenttime.tv_sec * 1000000) + (currenttime.tv_usec)) - ((starttime.tv_sec * 1000000) + (starttime.tv_usec))  ) < 4000000)
      {
	if (LightValue>400&&(up==0))
	{
	//  printf("Up detected");
	  up=1;
	}
	else if (LightValue<100&&(up==1))
	{ counter++;
	//printf("Down detected");
	up=0;
	}
	gettimeofday(&currenttime,NULL);
      }
    
      //Need to divide frequency by 2
      
      frequency = (double)counter/4;
      //printf("\n\n\n\nThe counter is \n%d\n\n\n\n",counter);
    //printf("\n\n\n\nThe frequency is \n%f\n\n\n\n",frequency);
      cout<<"The frequency is "<<frequency<<endl;
      if(frequency>0 && frequency <1){
	action.frequencyHalfHzMovement();
      }
      else if(frequency >=0.8 && frequency <1.5){
	action.frequency1HzMovement();
      }
      else if(frequency >=1.5 && frequency <3.5){
	action.frequency2HzMovement();
      }
      else if(frequency >= 3.5 && frequency <5.5){
	action.frequency4HzMovement();
      }
      else if(frequency >=5.5 && frequency <7.5){
	action.frequency6HzMovement();
      }
      else if(frequency >=7.5 && frequency <12){
	action.frequency8HzMovement();
      }
      navigSpeed=100.00;
      //leaveBlackZone();
      
      robotStatus=2;
      
      check_frequency=0;
      frequency=0;
      
      
      
      pthread_exit(NULL);

}
*/