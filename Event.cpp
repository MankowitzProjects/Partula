/*
 * File:   Event.cpp
 * Author: daniel
 *
 * Created on December 4, 2011, 4:18 PM
 */

#include "Event.h"

EVENT currentEvent;

Event g_eventCenter;
extern SensorController g_sensorCtrl;

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
    switch (input.type)
    {
    case TYPE_INPUT_SWITCH:
        {
            return genSwitchEvent(input);
        }
    case TYPE_INPUT_SENSOR:
        {
            return genSensorEvent(input);
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
    if (STATE_ON == input.value)
    {
#if (DEBUG_MODE)
        cout << "genBumperEvent - " << GetPositionChar(input.pos) << "bumper HIT!\n" << endl;
#endif

        switch (input.pos)
        {
        case POSITION_FRONT:
            {
                cout<<"Position Front"<<endl;
                return EVENT_HIT_FRONT;
            }
        case POSITION_LEFT:
            {
                cout<<"Position Left"<<endl;
                return EVENT_HIT_FRONT_LEFT;
            }

        case POSITION_RIGHT:
            {
                cout<<"Position right"<<endl;
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
    case POSITION_UNDER:
        {
            // test the value against the known value
            return GenLightSensorUnderEvent(input);
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

EVENT Event::GenLightSensorUnderEvent(const INPUT &input)
{
    int valueAvrg = g_sensorCtrl.getSensorValueAvrg(input.index);

    // if the value range is in the black paper and black tape
    if (bIsBlackTape(valueAvrg) || bIsBlackPaper(valueAvrg))
    {
        return EVENT_DETECT_BLACK;
    }
    else if (bIsGround(valueAvrg))
    {
        return EVENT_DETECT_GROUND;
    }
    else
    {
        return EVENT_DETECT_GROUND;
    }
}


void Event::handleInput(const INPUT &input)
{
    //cout<<"Handling Input"<<endl;
    currentEvent = checkEventType(input);
    cout<<"Current Event: "<<currentEvent<<endl;
    
}
