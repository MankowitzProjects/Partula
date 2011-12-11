/*
 * File:   Handle.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:44 PM
 */

#ifndef HANDLE_H
#define	HANDLE_H


#include "conf.h"
#include <sys/time.h>
#include "Action.h"
#include "ServoController.h"
#include <sys/time.h>
#include "Pose.h"
#include "MotorController.h"
//#include "Frequency.h"

class Handle {
public:
    Handle();
    Handle(const Handle& orig);
    virtual ~Handle();

    void collision();
    void docking();
    void localization();
    void triggerSwitch();
    void reScan();

    void fr_check(void);
    bool scanArea(bool foundFlag);

    //Frequency frequency(3);
private:

};

#endif	/* HANDLE_H */

