#ifndef DEBUG_TOOLS_H_INCLUDED
#define DEBUG_TOOLS_H_INCLUDED

using namespace std;

// change it to 0 if run on robot!!!
#define DEBUG_MODE_PC                   1

#define DEBUG_MODE                      1
#define DEBUG_MODE_LOW                  0
#define DEBUG_MODE_MOTOR                0
#define DEBUG_MODE_EVENT                0
#define DEBUG_MODE_SENSOR_CONTROLLER    0
#define DEBUG_SENSOR                    1
#define DEBUG_MODE_STEP_INSTRUCTION     1

static inline void prt(const char *msg)
{
    printf("%s", msg);
}

#if (DEBUG_MODE)
static inline void prt_debug(const char *msg)
{
    printf("%s", msg);
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
