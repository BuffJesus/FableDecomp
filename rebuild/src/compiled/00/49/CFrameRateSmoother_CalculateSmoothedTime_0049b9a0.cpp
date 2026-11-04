#include "fable_game.h"
void CFrameRateSmoother::CalculateSmoothedTime() {
    float total = 0.0f;
    long index = FirstTime;
    for (long count=0; count<NoTimes; ++count) { total += Times[index]; index = (index+1) % MaxNoTimes; }
    SmoothedTime = total / NoTimes;
}