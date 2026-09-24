#include "fable_frontend_component_types.h"

void CStopWatch::Start()
{
    // Start resumes accumulated time; calling it while running rebases the clock.
    m_nStartTick = GetTicks();
    m_bIsRunning = true;
}
