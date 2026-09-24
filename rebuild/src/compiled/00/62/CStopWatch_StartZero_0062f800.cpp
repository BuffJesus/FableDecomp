#include "fable_frontend_component_types.h"

void CStopWatch::StartZero()
{
    m_nPrevElapsedTicks = 0;
    m_nStartTick = GetTicks();
    m_bIsRunning = true;
}
