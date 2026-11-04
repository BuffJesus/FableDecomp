#include "fable_game.h"
void CFrameRateSmoother::AddFrameTime(double time) {
    if (NoTimes == MaxNoTimes) { FirstTime = (FirstTime+1) % MaxNoTimes; --NoTimes; }
    Times[LastTime] = static_cast<float>(time);
    LastTime = (LastTime+1) % MaxNoTimes;
    ++NoTimes;
    CalculateSmoothedTime();
}