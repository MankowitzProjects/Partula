#ifndef STATUS_H_INCLUDED
#define STATUS_H_INCLUDED

using namespace std;


typedef enum
{
    STATUS_NULL = 0x00000000,
    STATUS_EXPLORING,
    STATUS_DOCKING,
    STATUS_SCANNING,
    STATUS_LEAVING,

    STATUS_BUTT
} STATUS;

#endif // STATUS_H_INCLUDED
