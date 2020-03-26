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
    unsigned int x;
    unsigned int y;
} snapshotFrame;

class Logger
{
    public:
        Logger(){snapCount = reserveSize = 0;}
        Logger(unsigned int reserveSize);
        void Add(unsigned int x, unsigned int y, unsigned int angle, unsigned int distance);
        snapshotFrame GetLastFrame(){return snapshotFrames[snapCount-1];}
        snapshotFrame GetFrame(unsigned int id){return snapshotFrames[id];}
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