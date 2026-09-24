#include "fable_frontend_component_types.h"
#include "fable_performance_counter.h"

__int64 CStopWatch::GetTicks() const
{
    FablePerformanceCounterValue ticks;
    return QueryPerformanceCounter(&ticks) ? ticks.QuadPart : 0;
}
