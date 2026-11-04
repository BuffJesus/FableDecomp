#include "fable_stopwatch.h"
void CStopWatch::Reset()
{
    m_nPrevElapsedTicks = 0; if (m_bIsRunning) m_nStartTick = GetTicks();
}