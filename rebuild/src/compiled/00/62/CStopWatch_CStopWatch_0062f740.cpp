#include "fable_stopwatch.h"
CStopWatch::CStopWatch()
    : m_fTimerPeriod(0.0f), m_nStartTick(0), m_nPrevElapsedTicks(0), m_bIsRunning(false)
{
    __int64 frequency;
    if (QueryPerformanceFrequency(&frequency))
        m_fTimerPeriod = 1.0f / frequency;
    else
        m_fTimerPeriod = 1.0f;
}