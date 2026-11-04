#include "fable_game.h"

bool CFrameRateSmoother::Init(long maxNoTimes, long noFramesToChangeFrameRateOver)
{
    if (CInitBaseClass::Initialise())
    {
        Times.resize(maxNoTimes);
        SmoothedTime = 0.1f;
        MaxNoTimes = maxNoTimes;
        FirstTime = 0;
        LastTime = 0;
        NoTimes = 0;
        NoFramesToChangeFrameRateOver = noFramesToChangeFrameRateOver;
        return true;
    }
    return false;
}