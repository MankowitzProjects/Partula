#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

#include "conf.h"

class Switch
{
public:
    Switch(void)
    {
        init();
    };

    void init(void);
    int  init(int index, TYPE_SWITCH type, POSITION pos);
    void fin(void);

    inline void         setType(TYPE_SWITCH type);
    inline TYPE_SWITCH  getType(void);

    inline void         setPos(POSITION pos);
    inline POSITION     getPos(void);

    inline void  setValue(int value);
    inline int   getValue(void);
    inline int   getValuePre(void);

private:

    int index;          /**< The index of the Switch */

    TYPE_SWITCH type;   /**< The Switch type */
    POSITION    pos;    /**< The position where the Switch is installed */

    int value;          /**< The current value reported from the Switch */
    int valuePre;       /**< The previous value */
};


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

inline void Switch::setType(TYPE_SWITCH type)
{
    this->type = type;
}

inline TYPE_SWITCH Switch::getType(void)
{
    return type;
}

inline void Switch::setPos(POSITION pos)
{
    this->pos = pos;
}

inline POSITION Switch::getPos(void)
{
    return pos;
}

inline void Switch::setValue(int value)
{
    valuePre    = this->value;
    this->value = value;
}

inline int Switch::getValue(void)
{
    return value;
}

inline int Switch::getValuePre(void)
{
    return valuePre;
}


#endif // SWITCH_H_INCLUDED
