/*
 * File:   Event.cpp
 * Author: daniel
 *
 * Created on December 4, 2011, 4:18 PM
 */

#include "Event.h"

EVENT currentEvent;

Event g_eventCenter;

Event::Event()
{
}

Event::Event(const Event& orig)
{
}

Event::~Event()
{
}

EVENT Event::checkEventType(const INPUT &input)
{
    cout << "checkEventType" << endl;

    switch (input.type)
    {
    case TYPE_INPUT_SWITCH:
    {
        genSwitchEvent(input);
        break;
    }
    case TYPE_INPUT_SENSOR:
    {
        genSensorEvent(input);
        break;
    }
    default:
    {
        break;
    }
    }

    return EVENT_NULL;
}

EVENT Event::genSwitchEvent(const INPUT &input)
{
    cout << "genSwitchEvent" << endl;

    switch (input.subType)
    {
    case TYPE_SWITCH_BUMPER:
        {
            return genBumperEvent(input);
        }
    case TYPE_SWITCH_WHISKER:
        {
            return EVENT_NULL;
        }
    default:
        {
            printf("genSwitchEvent - switch %d with unknown type: %d\n", input.index, input.subType);
        }
    }

    return EVENT_NULL;
}


EVENT Event::genBumperEvent(const INPUT &input)
{
    cout << "genBumperEvent" << endl;
    cout << input.index << endl;
    cout << input.type << endl;
    cout << input.subType << endl;
    cout << input.pos << endl;
    cout << input.value << endl;

    if (STATE_ON == input.value)
    {
#if (DEBUG_MODE)
        cout << "genBumperEvent - ";
        cout << GetPositionChar(input.pos) << "bumper HIT!\n" << endl;
#endif

        switch (input.pos)
        {
        case POSITION_FRONT:
            {
                return EVENT_HIT_FRONT;
            }
        case POSITION_LEFT:
            {
                return EVENT_HIT_FRONT_LEFT;
            }

        case POSITION_RIGHT:
            {
                return EVENT_HIT_FRONT_RIGHT;
            }
        default:
            {
                printf("genBumperEvent: unknown bumper HIT!\n");
                break;
            }
        }
    }

    return EVENT_HIT;
}


EVENT Event::genSensorEvent(const INPUT &input)
{
    switch (input.subType)
    {
    case TYPE_SENSOR_IR:
        {
            return genIREvent(input);
        }
    case TYPE_SENSOR_LIGHT:
        {
            return genLightSensorEvent(input);
        }
    default:
        {
            printf("genSensorEvent: unknown sensor type: %d\n", input.subType);
            break;
        }
    }

    return EVENT_NULL;
}

EVENT Event::genIREvent(const INPUT &input)
{
    return EVENT_NULL;
}

EVENT Event::genLightSensorEvent(const INPUT &input)
{
    switch (input.pos)
    {
    case POSITION_FRONT:
        {
            genLightSensorFrontEvent(input);
            break;
        }
    default:
        {
            printf("genLightSensorEvent: light sensor %d, unkown position: %s.\n", input.index, GetPositionChar(input.pos));
            break;
        }
    }

    return EVENT_NULL;
}


EVENT Event::genLightSensorFrontEvent(const INPUT &input)
{
    /*if (input.value < VALUE_MIN_LED_LIGHT)
    {
        return EVENT_NULL;
    }

    int valuePre = GetSensorValuePre(input.index);

    if (input.value >= valuePre)
    {
        g_lightValuePeak = input.value;
    }
    else if (input.value < valuePre)
    {
        if (g_lightValuePeak == valuePre)
        {
            FreqTick();
            g_lightValuePeak = 0;
        }
    }
    else
    {
        ;
    }*/

    return EVENT_NULL;
}

void Event::handleInput(const INPUT &input)
{
    cout << "handleInput" << endl;
    currentEvent = checkEventType(input);
}
