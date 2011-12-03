#include "Switch.h"


/** \brief Default init functoin, set the values to null
 *
 * \param void
 * \return void
 *
 */
void Switch::init(void)
{
    type     = TYPE_SWITCH_NULL;
    pos      = POSITION_NULL;
    value    = 0;
    valuePre = 0;
}

/** \brief Initialize the Switch
 *
 * \param index int         The prot of the Switch
 * \param type TYPE_SWITCH  The type of the Switch
 * \param pos POSITION      The position of the Switch is installed
 * \return int              The result of the initialization
 *
 */
int Switch::init(int index, TYPE_SWITCH type, POSITION pos)
{
#if (DEBUG_MODE)
    printf("Switch::init, type: %s, position: %s\n", GetSwitchTypeChar(type), GetPositionChar(pos));
#endif

    this->index = index;
    this->type  = type;
    this->pos   = pos;

    return 0;
}

/** \brief Finalize the Switch
 *
 * \param void
 * \return void
 *
 */
void Switch::fin(void)
{
    init();
}
