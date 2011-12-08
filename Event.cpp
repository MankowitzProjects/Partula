/*
 * File:   Event.cpp
 * Author: daniel
 *
 * Created on December 4, 2011, 4:18 PM
 */

#include "Event.h"
#include "Frequency.h"
#include "debug_tools.h"

EVENT currentEvent;
EVENT previousEvent = EVENT_NULL;

Event g_eventCenter;
extern SensorController g_sensorCtrl;

int g_lightValuePeakLeft;
int g_lightValuePeakMiddle;
int g_lightValuePeakRight;

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
           // printf("genSwitchEvent - switch %d with unknown type: %d\n", input.index, input.subType);
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
                //printf("genBumperEvent: unknown bumper HIT!\n");
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
    case TYPE_SENSOR_SONAR:
    {
        // cout << << endl;
        break;
    }
    default:
        {
            //printf("genSensorEvent: unknown sensor type: %d\n", input.subType);
            break;
        }

    }

    return EVENT_NULL;
}

EVENT Event::genIREvent(const INPUT &input)
{
    #if DEBUG_MODE_SENSOR_IR
    cout << "genIREvent - " << GetPositionChar(input.pos) << " IR sensor: " << input.value << endl;
    #endif
    return EVENT_NULL;
}

EVENT Event::genLightSensorEvent(const INPUT &input)
{
    switch (input.pos)
    {
    case POSITION_MIDDLE:
    case POSITION_LEFT:
    case POSITION_RIGHT:
    {
        return genLightSensorFrontEvent(input);
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
    /*
    if (input.value < VALUE_MIN_LED_LIGHT)
    {
        return EVENT_NULL;
    }
    */

    int valuePre = GetSensorValuePre(input.index);

    #if DEBUG_SENSOR_LIGHT_FRONT
    cout << "genLightSensorFrontEvent - value:     " << input.value << endl;
    cout << "genLightSensorFrontEvent - value pre: " << valuePre << endl;
    #endif

    // update the peak value
    if (input.value >= valuePre)
    {
        if (POSITION_LEFT == input.pos)
        {
            g_lightValuePeakLeft = input.value;
        }
        else if (input.pos ==  POSITION_MIDDLE)
        {
            g_lightValuePeakMiddle = input.value;
        }
        else if (input.pos == POSITION_RIGHT)
        {
            g_lightValuePeakRight = input.value;
        }
        else
        {
            ;
        }
    }

    // the trend is going down
    else if (input.value < valuePre)
    {
        switch (input.pos)
        {
        case POSITION_LEFT:
        {
            // previous is the peak
            if (g_lightValuePeakLeft == valuePre)
            {
                FreqTickLeft();

                // reset the peak
                g_lightValuePeakLeft = 0;
            }
            break;
        }
        case POSITION_MIDDLE:
        {
            // previous is the peak
            if (g_lightValuePeakMiddle == valuePre)
            {
                FreqTickMiddle();

                // reset the peak
                g_lightValuePeakMiddle = 0;
            }
            break;
        }
        case POSITION_RIGHT:
        {
            // previous is the peak
            if (g_lightValuePeakRight == valuePre)
            {
                FreqTickRight();

                // reset the peak
                g_lightValuePeakRight = 0;
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    else
    {
        ;
    }

    return EVENT_NULL;
}

EVENT Event::GenLightSensorUnderEvent(const INPUT &input)
{
    int valueAvrg = g_sensorCtrl.getSensorValueAvrg(input.index);
    int value = g_sensorCtrl.getSensorValue(input.index);
    // if the value range is in the black paper and black tape
    if (bIsBlackTape(value)) //|| bIsBlackPaper(valueAvrg))
    {
        //cout<<"Average Value: "<<value<<endl;
        //cout<<"Got to BLACK TAPE EVENT with sensor value: "<<g_sensorCtrl.getSensorValue(input.index)<<endl;
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

    if (currentEvent != previousEvent)
    {
        // cout<<"Current Event: "<<currentEvent<<endl;
        previousEvent = currentEvent;
    }
}
