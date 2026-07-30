#include "fable_gfmain.h"
#include "fable_profile.h"

long FABLE_FASTCALL FableRunGFMainPhase10(
    const GFMainPhase9State& phase9,
    GFMainPhase10State& state)
{
    if (phase9.observedSystemInitialiseResult == 1)
    {
        if (FableGFMainPhase10GFInitialiseBoundary(state))
        {
            if (FableGFMainPhase10HasStartupMessageBoundary(state))
            {
                FableGFMainPhase10GetRenderManagerBoundary(state);
                FableGFMainPhase10AddStartupLineBoundary(state);
            }

            NProfileTimer::EndProfile(0);
            ++state.endProfileCalls;
            FableGFMainPhase10PlayBoundary(state);
            FableGFMainPhase10GFUninitialiseBoundary(state);
        }
    }
    else
    {
        FableGFMainPhase10HandleSystemInitErrorBoundary(
            phase9.observedSystemInitialiseResult,
            state);
    }

    NProfileTimer::EndProfile(0);
    ++state.endProfileCalls;

    FableGFMainPhase10ConstructRegistryBoundary(state);
    FableGFMainPhase10WriteRegistryBoolBoundary(false, state);
    FableGFMainPhase10DestroyRegistryBoundary(state);

    // These nine owners span earlier retail clusters. The authored phase
    // units release their local strings at their own lexical boundaries, so
    // this state records retail's final cleanup cardinality without inventing
    // duplicate owners merely to replay the destructor calls.
    state.wideStringCleanupCalls = 7;
    state.charStringCleanupCalls = 2;
    return 0;
}
