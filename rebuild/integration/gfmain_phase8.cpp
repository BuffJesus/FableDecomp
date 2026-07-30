#include "fable_gfmain.h"

long FABLE_FASTCALL FableRunGFMainPhase8(
    const GFMainPhase7State& phase7,
    GFMainPhase8State& state)
{
    if (phase7.skipPhase8)
        return 0;

    if (!FableGFMainPhase8DoEULABoundary(state))
    {
        FableGFMainPhase8ProcessExitBoundary(1, state);
        return 1;
    }

    if (!phase7.configuredSkipConfigDetection)
    {
        if (!FableGFMainPhase8DetectConfigurationBoundary(state))
        {
            FableGFMainPhase8ProcessExitBoundary(1, state);
            return 1;
        }
        FableGFMainPhase8FreeConfigurationBoundary(state);
    }

    return 0;
}
