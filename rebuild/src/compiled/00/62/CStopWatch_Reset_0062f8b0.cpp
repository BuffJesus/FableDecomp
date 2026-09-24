#include "fable_frontend_component_types.h"

void CStopWatch::Reset()
{
    m_nPrevElapsedTicks = 0;
    if (m_bIsRunning)
        m_nStartTick = GetTicks();
}
