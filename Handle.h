/* 
 * File:   Handle.h
 * Author: daniel
 *
 * Created on December 3, 2011, 9:44 PM
 */

#ifndef HANDLE_H
#define	HANDLE_H

class Handle {
public:
    Handle();
    Handle(const Handle& orig);
    virtual ~Handle();
    void collision();
    void docking();
    void localization();
private:

};

#endif	/* HANDLE_H */
