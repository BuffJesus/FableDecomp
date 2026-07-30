#include <stdio.h>
#include <string.h>

#include "fable_gfmain.h"

namespace
{
    void Reset(
        GFMainPhase9State& phase9,
        GFMainPhase10State& state)
    {
        memset(&phase9, 0, sizeof(phase9));
        memset(&state, 0, sizeof(state));
        phase9.observedSystemInitialiseResult = 1;
        state.gfInitialiseSucceeds = true;
        state.startupMessagePresent = true;
    }

    bool FinalCleanupMatches(const GFMainPhase10State& state)
    {
        return
            state.registryConstructCalls == 1 &&
            state.registryWriteCalls == 1 &&
            state.registryDestroyCalls == 1 &&
            !state.registryWrittenValue &&
            state.wideStringCleanupCalls == 7 &&
            state.charStringCleanupCalls == 2;
    }
}

int main()
{
    GFMainPhase9State phase9;
    GFMainPhase10State state;

    Reset(phase9, state);
    if (
        FableRunGFMainPhase10(phase9, state) != 0 ||
        state.gfInitialiseCalls != 1 ||
        !state.observedGFInitialiseResult ||
        state.startupMessageCheckCalls != 1 ||
        state.getRenderManagerCalls != 1 ||
        state.addStartupLineCalls != 1 ||
        state.playCalls != 1 ||
        state.gfUninitialiseCalls != 1 ||
        state.systemInitErrorCalls != 0 ||
        state.endProfileCalls != 2 ||
        !FinalCleanupMatches(state))
    {
        puts("FABLETLC_GFMAIN_PHASE10_BEHAVIOR FAIL code=1");
        return 1;
    }

    Reset(phase9, state);
    state.startupMessagePresent = false;
    if (
        FableRunGFMainPhase10(phase9, state) != 0 ||
        state.gfInitialiseCalls != 1 ||
        state.startupMessageCheckCalls != 1 ||
        state.getRenderManagerCalls != 0 ||
        state.addStartupLineCalls != 0 ||
        state.playCalls != 1 ||
        state.gfUninitialiseCalls != 1 ||
        state.endProfileCalls != 2 ||
        !FinalCleanupMatches(state))
    {
        puts("FABLETLC_GFMAIN_PHASE10_BEHAVIOR FAIL code=2");
        return 2;
    }

    Reset(phase9, state);
    state.gfInitialiseSucceeds = false;
    if (
        FableRunGFMainPhase10(phase9, state) != 0 ||
        state.gfInitialiseCalls != 1 ||
        state.observedGFInitialiseResult ||
        state.startupMessageCheckCalls != 0 ||
        state.playCalls != 0 ||
        state.gfUninitialiseCalls != 0 ||
        state.systemInitErrorCalls != 0 ||
        state.endProfileCalls != 1 ||
        !FinalCleanupMatches(state))
    {
        puts("FABLETLC_GFMAIN_PHASE10_BEHAVIOR FAIL code=3");
        return 3;
    }

    Reset(phase9, state);
    phase9.observedSystemInitialiseResult = 4;
    if (
        FableRunGFMainPhase10(phase9, state) != 0 ||
        state.gfInitialiseCalls != 0 ||
        state.systemInitErrorCalls != 1 ||
        state.observedSystemInitError != 4 ||
        state.endProfileCalls != 1 ||
        !FinalCleanupMatches(state))
    {
        puts("FABLETLC_GFMAIN_PHASE10_BEHAVIOR FAIL code=4");
        return 4;
    }

    puts("FABLETLC_GFMAIN_PHASE10_BEHAVIOR PASS");
    return 0;
}
