/* 
 * File:   Event.h
 * Author: daniel
 *
 * Created on December 4, 2011, 4:18 PM
 */

#ifndef EVENT_H
#define	EVENT_H

#include "conf.h"

class Event {
public:
    Event();
    Event(const Event& orig);
    virtual ~Event();
    
    void  handleEvent();
    void  handleInput(const INPUT &input);
private:
    EVENT checkEventType(const INPUT &input);
    
    EVENT genIREvent(const INPUT &input);
    EVENT genLightSensorEvent(const INPUT &input);
    EVENT genLightSensorFrontEvent(const INPUT &input);
    EVENT genSensorEvent(const INPUT &input);

    EVENT genSwitchEvent(const INPUT &input);
    EVENT genBumperEvent(const INPUT &input);
};

#endif	/* EVENT_H */

