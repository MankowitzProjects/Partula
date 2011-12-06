#include "Frequency.h"

Frequency g_freqLight(FREQUENCY_DEFAULT_SAMPLE_RATE);

void FreqTick(void)
{
    g_freqLight.tick();
}
float FreqGetFreqency(void)
{
    return g_freqLight.getFrequency();
}

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

Frequency::Frequency(void)
{
    Frequency(FREQUENCY_DEFAULT_SAMPLE_RATE);
}

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

Frequency::~Frequency(void)
{
    if (0 != tm_array)
    {
        free(tm_array);
        tm_array = 0;
    }
}

