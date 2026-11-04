#include "fable_stopwatch.h"
void CStopWatch::Start()
{
    m_nStartTick = GetTicks(); m_bIsRunning = true;
}