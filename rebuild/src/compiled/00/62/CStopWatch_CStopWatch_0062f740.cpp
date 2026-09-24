#include "fable_frontend_component_types.h"
#include "fable_performance_counter.h"

CStopWatch::CStopWatch()
    : m_fTimerPeriod(0), m_nStartTick(0), m_nPrevElapsedTicks(0), m_bIsRunning(false)
{
    FablePerformanceCounterValue frequency;
    m_fTimerPeriod = QueryPerformanceFrequency(&frequency)
        ? static_cast<float>(1.0 / static_cast<double>(frequency.QuadPart)) : 1.0f;
}
