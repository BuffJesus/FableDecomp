#include <stdio.h>
#include <string.h>

#include "fable_gfmain.h"

namespace
{
    void Reset(
        GFMainPhase7State& phase7,
        GFMainPhase8State& state)
    {
        memset(&phase7, 0, sizeof(phase7));
        memset(&state, 0, sizeof(state));
        state.eulaSucceeds = true;
        state.configurationSucceeds = true;
    }

    bool ExactCountersAreZero(const GFMainPhase8State& state)
    {
        return
            state.exactEulaExportCalls == 0 &&
            state.exactEulaFreeCalls == 0 &&
            state.exactConfigurationCreateCalls == 0 &&
            state.exactConfigurationVideoCalls == 0 &&
            state.exactConfigurationRequirementsCalls == 0 &&
            state.exactConfigurationReleaseCalls == 0 &&
            state.exactConfigurationFreeCalls == 0 &&
            state.exactConfigurationSecurityChecks == 0;
    }
}

int main()
{
    GFMainPhase7State phase7;
    GFMainPhase8State state;

    Reset(phase7, state);
    if (
        FableRunGFMainPhase8(phase7, state) != 0 ||
        state.eulaCalls != 1 ||
        state.configurationCalls != 1 ||
        state.freeConfigurationCalls != 1 ||
        state.processExitCalls != 0 ||
        !ExactCountersAreZero(state))
    {
        puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR FAIL code=1");
        return 1;
    }

    Reset(phase7, state);
    state.eulaSucceeds = false;
    if (
        FableRunGFMainPhase8(phase7, state) != 1 ||
        state.eulaCalls != 1 ||
        state.configurationCalls != 0 ||
        state.freeConfigurationCalls != 0 ||
        state.processExitCalls != 1 ||
        state.requestedExitCode != 1)
    {
        puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR FAIL code=2");
        return 2;
    }

    Reset(phase7, state);
    state.configurationSucceeds = false;
    if (
        FableRunGFMainPhase8(phase7, state) != 1 ||
        state.eulaCalls != 1 ||
        state.configurationCalls != 1 ||
        state.freeConfigurationCalls != 0 ||
        state.processExitCalls != 1 ||
        state.requestedExitCode != 1)
    {
        puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR FAIL code=3");
        return 3;
    }

    Reset(phase7, state);
    phase7.configuredSkipConfigDetection = true;
    if (
        FableRunGFMainPhase8(phase7, state) != 0 ||
        state.eulaCalls != 1 ||
        state.configurationCalls != 0 ||
        state.freeConfigurationCalls != 0 ||
        state.processExitCalls != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR FAIL code=4");
        return 4;
    }

    Reset(phase7, state);
    phase7.skipPhase8 = true;
    if (
        FableRunGFMainPhase8(phase7, state) != 0 ||
        state.eulaCalls != 0 ||
        state.configurationCalls != 0 ||
        state.freeConfigurationCalls != 0 ||
        state.processExitCalls != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR FAIL code=5");
        return 5;
    }

    puts("FABLETLC_GFMAIN_PHASE8_BEHAVIOR PASS");
    return 0;
}
