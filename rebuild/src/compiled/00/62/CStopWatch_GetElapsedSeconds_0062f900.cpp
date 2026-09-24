#include "fable_frontend_component_types.h"

float CStopWatch::GetElapsedSeconds() const
{
    __int64 elapsed = m_nPrevElapsedTicks;
    if (m_bIsRunning)
        elapsed += GetTicks() - m_nStartTick;
    return static_cast<float>(static_cast<double>(elapsed) * m_fTimerPeriod);
}
