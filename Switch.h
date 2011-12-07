#ifndef SWITCH_H_INCLUDED
#define SWITCH_H_INCLUDED

#include "conf.h"

using namespace std;

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

    inline bool  isExist(void) {return bIsInit;}

private:

    int index;          /**< The index of the Switch */

    TYPE_SWITCH type;   /**< The Switch type */
    POSITION    pos;    /**< The position where the Switch is installed */

    int value;          /**< The current value reported from the Switch */
    int valuePre;       /**< The previous value */

    bool bIsInit;
};


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
