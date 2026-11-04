#include "fable_stopwatch.h"
void CStopWatch::Stop()
{
    if (m_bIsRunning) { m_nPrevElapsedTicks += GetTicks() - m_nStartTick; m_nStartTick = 0; m_bIsRunning = false; }
}