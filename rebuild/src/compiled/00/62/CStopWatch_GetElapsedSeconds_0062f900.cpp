#include "fable_stopwatch.h"
float CStopWatch::GetElapsedSeconds() const
{
    __int64 elapsed = m_nPrevElapsedTicks; if (m_bIsRunning) elapsed += GetTicks() - m_nStartTick; return elapsed * m_fTimerPeriod;
}