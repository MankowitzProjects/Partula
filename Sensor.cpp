#include "Sensor.h"



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
    bIsInit  = false;
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

    bIsInit = true;

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
