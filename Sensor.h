#ifndef SENSOR_H_INCLUDED
#define SENSOR_H_INCLUDED

#include "conf.h"

class Sensor
{
public:
    Sensor(void)
    {
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
    inline int   getValuePre(void);

private:

    int index;          /**< The index of the sensor */

    TYPE_SENSOR type;   /**< The sensor type */
    POSITION    pos;    /**< The position where the sensor is installed */

    int value;          /**< The current value reported from the sensor */
    int valuePre;       /**< The previous value */
};


/** \brief Default init functoin, set the values to null
 *
 * \param void
 * \return void
 *
 */
void Sensor::init(void)
{
    type     = TYPE_SENSOR_NULL;
    pos      = POSITION_NULL;
    value    = 0;
    valuePre = 0;
}

/** \brief Initialize the sensor
 *
 * \param index int         The prot of the sensor
 * \param type TYPE_SENSOR  The type of the sensor
 * \param pos POSITION      The position of the sensor is installed
 * \return int              The result of the initialization
 *
 */
int Sensor::init(int index, TYPE_SENSOR type, POSITION pos)
{
#if (DEBUG_MODE)
    printf("Sensor::init, type: %s, position: %s\n", GetSensorTypeChar(type), GetPositionChar(pos));
#endif

    this->index = index;
    this->type  = type;
    this->pos   = pos;

    return 0;
}

/** \brief Finalize the sensor
 *
 * \param void
 * \return void
 *
 */
void Sensor::fin(void)
{
    init();
}

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
    valuePre    = this->value;
    this->value = value;
}

inline int Sensor::getValue(void)
{
    return value;
}

inline int Sensor::getValuePre(void)
{
    return valuePre;
}

#endif // SENSOR_H_INCLUDED
