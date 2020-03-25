#include "logger.h"

//logger will log relevant data through the simulation
//then at certain intervals log that data to a file

Logger::Logger(unsigned int reserveSize)
{
    this->reserveSize = reserveSize;
    snapCount = 0;
    snapshotFrames = new snapshotFrame[reserveSize];
}

void Logger::WriteToFile()
{
    FILE* f = fopen("LOG.TXT", "wb");
    if (f){
        fwrite(&snapshotFrames[0], sizeof(snapshotFrame) * reserveSize, 1, f);
        fclose(f);
        return;
    }
}

void Logger::Add(unsigned int angle, unsigned int distance)
{
    //if we've exceeded out reserve size, start again
    if (snapCount >= reserveSize){
        WriteToFile();
        snapCount = 0;
        memset(&snapshotFrames[0], 0, sizeof(snapshotFrame) * reserveSize);
    }

    else
    {
        snapshotFrame snap;
        //get all the neccessary data for logging
        time(&rawTime);
        timeInfo = localtime(&rawTime);
        strftime(snap.shortdateTime, 32, "%x|%X", timeInfo);
        snap.angle = angle;
        snap.distance = distance;
        //push back the new data
        snapshotFrames[snapCount] = snap;
        snapCount++;
    }
}

Logger::~Logger()
{
    delete snapshotFrames;
}