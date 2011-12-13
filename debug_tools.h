#ifndef DEBUG_TOOLS_H_INCLUDED
#define DEBUG_TOOLS_H_INCLUDED

#include <iostream>

using namespace std;

// change it to 0 if run on robot!!!
#define DEBUG_MODE_PC                   0

#define DEBUG_MODE                      0
#define DEBUG_MODE_LOW                  0
#define DEBUG_MODE_MOTOR                0
#define DEBUG_MODE_EVENT                0
#define DEBUG_MODE_SENSOR_CONTROLLER    0
#define DEBUG_MODE_SERVO_CONTROLLER     0
#define DEBUG_SENSOR                    0
#define DEBUG_MODE_STEP_INSTRUCTION     1
#define DEBUG_SENSOR_LIGHT_FRONT        0
#define DEBUG_MODE_FREQUENCY            0
#define DEBUG_MODE_LOCALIZATION         0
#define DEBUG_MODE_SONAR_SCAN           1
#define DEBUG_MODE_SENSOR_IR            0
#define DEBUG_MODE_SCAN                 1

static inline void prt(const char *msg)
{
    cout << msg;
}

#if (DEBUG_MODE)
static inline void prt_debug(const char *msg)
{
    cout << msg;
}
#else
    #define prt_debug(n)
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
    #include <windows.h>
    #include "./include/phidget21.h"
    #define sleep(n) Sleep(n)
#else
    #include <time.h>
    #include <phidget21.h>
#endif

static inline int wait(unsigned long milisec)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
    sleep(milisec);
#else
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;

    while(nanosleep(&req,&req) == -1)
    {
        continue;
    }
#endif
    return 1;
}

#endif // DEBUG_TOOLS_H_INCLUDED
