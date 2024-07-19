#pragma once

#include <mutex>

struct SharedData 
{
    bool newDataAvailable = false;
    std::mutex mutex;
};
struct EndProgram
{
    bool hasEnded = false;
    std::mutex EndProgramMutex;
};
