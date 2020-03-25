#ifndef LOGGER_H
#define LOGGER_H

#include <time.h>
#include <string.h>
#include <stdio.h>

typedef struct snapshotFrame
{
    char shortdateTime[32];
    unsigned int angle;
    unsigned int distance;
} snapshotFrame;

class Logger
{
    public:
        Logger(){snapCount = reserveSize = 0;}
        Logger(unsigned int reserveSize);
        void Add(unsigned int angle, unsigned int distance);
        snapshotFrame GetLastFrame(){return snapshotFrames[snapCount-1];}
        unsigned int Size(){return snapCount;}
        ~Logger();
    private:
        unsigned int reserveSize;
        unsigned int snapCount;
        time_t rawTime;
        struct tm* timeInfo;
        snapshotFrame* snapshotFrames;

        void WriteToFile();
};

#endif