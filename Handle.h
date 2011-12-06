/* 
 * File:   Handle.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:44 PM
 */

#ifndef HANDLE_H
#define	HANDLE_H


#include <pthread.h>
#include <sys/time.h>

class Handle {
public:
    Handle();
    Handle(const Handle& orig);
    virtual ~Handle();
    void collision();
    void docking();
    void localization();
    void* frequency(void * param);
    void* scanArea(void* param);
private:

};

#endif	/* HANDLE_H */

