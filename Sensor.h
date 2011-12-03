#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

#include "conf.h"

using namespace std;

#define SENSOR_SAMPLE_RATE  1

class Sensor
{
public:
    Sensor(void)
    {
        iValuePos = 0;
        init();
    };

    void init(void);
    int  init(int index, TYPE_SENSOR type, POSITION pos);
    void fin(void);

    inline void         setType(TYPE_SENSOR type);
    inline TYPE_SENSOR  getType(void);

    inline void         setPos(POSITION pos);
    inline POSITION     getPos(void);

    inline void  setValue(int value);
    inline int   getValue(void);
    inline int   getValueAvrg(void);

private:

    int index;          /**< The index of the sensor */

    TYPE_SENSOR type;   /**< The sensor type */
    POSITION    pos;    /**< The position where the sensor is installed */

    int value;          /**< The current value reported from the sensor */
    int valuePre;       /**< The previous value */
    int valueArray[SENSOR_SAMPLE_RATE]; /**< Store several recent values in the array */
    int iValuePos;      /**< Current Position of the value array */
};


inline void Sensor::setType(TYPE_SENSOR type)
{
    this->type = type;
}

inline TYPE_SENSOR Sensor::getType(void)
{
    return type;
}

inline void Sensor::setPos(POSITION pos)
{
    this->pos = pos;
}

inline POSITION Sensor::getPos(void)
{
    return pos;
}

inline void Sensor::setValue(int value)
{
    this->value = value;

    valueArray[iValuePos] = value;

    iValuePos++;

    if (iValuePos == SENSOR_SAMPLE_RATE)
    {
        iValuePos = 0;
    }
}

inline int Sensor::getValue(void)
{
    return value;
}

inline int Sensor::getValueAvrg(void)
{
    int valueAvrg = valueArray[0];

    for (int i = 1; i < SENSOR_SAMPLE_RATE; ++i)
    {
        valueAvrg += valueArray[i];
    }

    #if (DEBUG_SENSOR)
    printf("Sensor::getValueAvrg - %d\n", valueAvrg/SENSOR_SAMPLE_RATE);
    #endif

    return valueAvrg/SENSOR_SAMPLE_RATE;
}


#endif // SENSOR_H_INCLUDED
