#ifndef FREQUENCY_H_INCLUDED
#define FREQUENCY_H_INCLUDED

#include <time.h>
#include <stdlib.h>

#define nanosecToMillisec(nano) (nano/1000000)
#define FREQUENCY_DEFAULT_SAMPLE_RATE 3

using namespace std;

inline timespec diffTime(const struct timespec &tm_start, const struct timespec &tm_end)
{
    struct timespec temp;

    // 需要借位
    if ((tm_end.tv_nsec - tm_start.tv_nsec) < 0)
    {
        temp.tv_sec  = tm_end.tv_sec - tm_start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + tm_end.tv_nsec - tm_start.tv_nsec;
    }
    else
    {
        temp.tv_sec  = tm_end.tv_sec  - tm_start.tv_sec;
        temp.tv_nsec = tm_end.tv_nsec - tm_start.tv_nsec;
    }

    return temp;
}

inline timespec addTime(const struct timespec &tm_start, const struct timespec &tm_end)
{
    struct timespec temp;

    // 需要进位
    if ((tm_end.tv_nsec + tm_start.tv_nsec) >= 1000000000)
    {
        temp.tv_nsec = tm_end.tv_nsec + tm_start.tv_nsec - 1000000000;
        temp.tv_sec  = tm_end.tv_sec  + tm_start.tv_sec  + 1;
    }
    else
    {
        temp.tv_nsec = tm_end.tv_nsec + tm_start.tv_nsec;
        temp.tv_sec  = tm_end.tv_sec  + tm_start.tv_sec;
    }

    return temp;
}

class Frequency
{
public:
    Frequency(unsigned int sampleRate);
    ~Frequency(void);

    void  setSampleRate(unsigned int sampleRate);
    float getFrequency(void);
    void  tick(void);

private:
    Frequency(void);

    unsigned int sampleRate;
    unsigned int curPos;

    struct timespec  tm_lst;
    struct timespec  tm_cur;
    struct timespec *tm_array;
};

void FreqTickLeft(void);
float FreqGetFrequencyLeft(void);
void FreqTickMiddle(void);
float FreqGetFrequencyMiddle(void);
void FreqTickRight(void);
float FreqGetFrequencyRight(void);
float FreqGetFrequency(void);


#endif // FREQUENCY_H_INCLUDED
