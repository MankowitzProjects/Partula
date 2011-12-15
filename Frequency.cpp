/**< Implement the frequency class and define three frequency classes for the
     light sensors set in front of the robot */

#include "Frequency.h"
#include "debug_tools.h"
#include <iostream>

/**< the frequency from the left light sensor */
Frequency g_freqLightLeft(FREQUENCY_DEFAULT_SAMPLE_RATE);
/**< the frequency from the middle light sensor */
Frequency g_freqLightMiddle(FREQUENCY_DEFAULT_SAMPLE_RATE);
/**< the frequency from the right light sensor */
Frequency g_freqLightRight(FREQUENCY_DEFAULT_SAMPLE_RATE);

/** \brief get current frequency
 *
 * \param void
 * \return float
 *
 */
float Frequency::getFrequency(void)
{
    float  f_sec = 0.0;
    struct timespec tm_sum = {0, 0};

    for (unsigned int i = 0; i < sampleRate; ++i)
    {
        tm_sum = addTime(tm_sum, tm_array[i]);
    }

    f_sec = (float)tm_sum.tv_nsec / 1000000000.0f + tm_sum.tv_sec;

    return (sampleRate / f_sec);
}

/** \brief tick, then the class will be signaled, and it can then calculate the time difference
 *
 * \param void
 * \return void
 *
 */
void Frequency::tick(void)
{
    // get current time
    clock_gettime(CLOCK_REALTIME, &tm_cur);

    if (curPos >= sampleRate)
    {
        curPos = 0;
    }

    tm_array[curPos++] = diffTime(tm_lst, tm_cur);

    tm_lst = tm_cur;
}

/** \brief set a new sampling rate
 *
 * \param sampleRate unsigned int
 * \return void
 *
 */
void Frequency::setSampleRate(unsigned int sampleRate)
{
    this->sampleRate = sampleRate;

    if (0 != tm_array)
    {
        free(tm_array);
        tm_array = 0;
    }

    tm_array = (timespec *)malloc(sampleRate * sizeof(struct timespec));

    for (curPos = 0; curPos < sampleRate; ++curPos)
    {
        tm_array[curPos].tv_sec  = 0;
        tm_array[curPos].tv_nsec = 0;
    }

    curPos = 0;
}

/** \brief the default constructor
 *
 * \param void
 *
 */
Frequency::Frequency(void)
{
    Frequency(FREQUENCY_DEFAULT_SAMPLE_RATE);
}

/** \brief the default constructor
 *
 * \param sampleRate unsigned int   the sampling rate
 *
 */
Frequency::Frequency(unsigned int sampleRate)
{
    // curPos     = 0; // been set to 0 in setSampleRate
    tm_array       = 0;

    tm_lst.tv_sec  = 0;
    tm_lst.tv_nsec = 0;
    tm_cur.tv_sec  = 0;
    tm_cur.tv_nsec = 0;

    setSampleRate(sampleRate);
}

/** \brief the default destructor
 *
 * \param void
 *
 */
Frequency::~Frequency(void)
{
    if (0 != tm_array)
    {
        free(tm_array);
        tm_array = 0;
    }
}

/** \brief Tick the left light sensor
 *
 * \param void
 * \return void
 *
 */
void FreqTickLeft(void)
{
    #if DEBUG_MODE_FREQUENCY
    cout << "FreqTickLeft" << endl;
    #endif
    g_freqLightLeft.tick();
}

/** \brief Get frequency from the left light sensor
 *
 * \param void
 * \return float
 *
 */
float FreqGetFrequencyLeft(void)
{
    return g_freqLightLeft.getFrequency();
}

/** \brief Tick the middle light sensor
 *
 * \param void
 * \return void
 *
 */
void FreqTickMiddle(void)
{
    #if DEBUG_MODE_FREQUENCY
    cout << "FreqTickMiddle" << endl;
    #endif
    g_freqLightMiddle.tick();
}

/** \brief Get frequency from the middle light sensor
 *
 * \param void
 * \return float
 *
 */
float FreqGetFrequencyMiddle(void)
{
    return g_freqLightMiddle.getFrequency();
}

/** \brief Tick the right light sensor
 *
 * \param void
 * \return void
 *
 */
void FreqTickRight(void)
{
    #if DEBUG_MODE_FREQUENCY
    cout << "FreqTickRight" << endl;
    #endif
    g_freqLightRight.tick();
}

/** \brief Get frequency from the right light sensor
 *
 * \param void
 * \return float
 *
 */
float FreqGetFrequencyRight(void)
{
    return g_freqLightRight.getFrequency();
}

/** \brief Get frequency from the front light sensors, only the one with
 *         reasonable value will be reported.
 *
 * \param void
 * \return float
 *
 */
float FreqGetFrequency(void)
{
    float freq = 0.0;

    // the frequency should be within a desired range
    if (   (FreqGetFrequencyRight() > 0.3)
        && (FreqGetFrequencyRight() < 10.0))
    {
        freq = FreqGetFrequencyRight();
    }
    else if (   (FreqGetFrequencyLeft() > 0.3)
             && (FreqGetFrequencyLeft() < 10.0))
    {
        freq = FreqGetFrequencyLeft();
    }
    else if (   (FreqGetFrequencyMiddle() > 0.3)
             && (FreqGetFrequencyMiddle() < 10.0))
    {
        freq = FreqGetFrequencyMiddle();
    }
    else
    {
        freq = 0.0;
    }

    #if DEBUG_MODE_FREQUENCY
    cout << "Freq left  : " << FreqGetFrequencyLeft() << endl;
    cout << "Freq middle: " << FreqGetFrequencyMiddle() << endl;
    cout << "Freq right : " << FreqGetFrequencyRight() << endl;

    cout << "Freq result: " << freq << endl;
    #endif

    return freq;
}

