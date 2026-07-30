#include <stdio.h>
#include <string.h>

#include "fable_gfmain.h"
#include "fable_movie.h"

namespace
{
    void Reset(GFMainPhase9State& state)
    {
        memset(&state, 0, sizeof(state));
        state.engineFlagSourceB = true;
        state.systemInitialiseResult = 1;
        state.systemWindowHandleToken = 0x00ABCDEFUL;
        g_FableMovieLeftAlignText_013C5A7C = false;
    }
}

int main()
{
    GFMainPhase9State state;

    Reset(state);
    state.initialEngineFlags = 0x10;
    if (
        FableRunGFMainPhase9(state) != 0 ||
        state.configuredEngineFlags != 0x10 ||
        state.movieLeftAlignApplied ||
        g_FableMovieLeftAlignText_013C5A7C ||
        state.systemInitialiseCalls != 1 ||
        state.systemManagerLookupCalls != 2 ||
        state.observedSystemInitialiseResult != 1 ||
        state.exactSystemPreInitialiseCalls != 0 ||
        state.exactSystemColoursConstructCalls != 0 ||
        state.exactSystemAllocationCalls != 0 ||
        state.exactSystemWindowInitialiseCalls != 0 ||
        state.exactSystemDisplayConstructCalls != 0 ||
        state.exactSystemInputConstructCalls != 0 ||
        state.exactSystemDriveInitialiseCalls != 0 ||
        state.exactSystemTextConstructCalls != 0 ||
        state.exactSystemMeshConstructCalls != 0 ||
        state.exactSystemComponentAddCalls != 0 ||
        state.systemManagerToken != 0x013CA618UL ||
        state.secondSystemManagerToken != 0x013CA618UL ||
        state.imePrimitiveToken == 0 ||
        state.imeInitialiseCalls != 1 ||
        state.observedImeWindowHandleToken != 0x00ABCDEFUL ||
        state.exactImeUiInitialiseCalls != 0 ||
        state.processSaveMetadataCalls != 0 ||
        state.exactSaveMetadataCoordinatorCalls != 0 ||
        state.exactSaveMetadataFindFirstCalls != 0 ||
        state.exactSaveMetadataSecurityChecks != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE9_BEHAVIOR FAIL code=1");
        return 1;
    }

    Reset(state);
    state.initialEngineFlags = 0x10;
    state.engineFlagSourceA = true;
    state.engineFlagSourceB = false;
    state.forceMovieLeftAlign = true;
    state.processSaveFileMetadata = true;
    if (
        FableRunGFMainPhase9(state) != 0 ||
        state.configuredEngineFlags != 0x13 ||
        !state.movieLeftAlignApplied ||
        !g_FableMovieLeftAlignText_013C5A7C ||
        state.systemInitialiseCalls != 1 ||
        state.systemManagerLookupCalls != 2 ||
        state.imeInitialiseCalls != 1 ||
        state.processSaveMetadataCalls != 1 ||
        state.exactSaveMetadataCoordinatorCalls != 0 ||
        state.exactSaveMetadataFindFirstCalls != 0 ||
        state.exactSaveMetadataSecurityChecks != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE9_BEHAVIOR FAIL code=2");
        return 2;
    }

    Reset(state);
    state.systemInitialiseResult = 4;
    if (
        FableRunGFMainPhase9(state) != 0 ||
        state.observedSystemInitialiseResult != 4 ||
        state.systemInitialiseCalls != 1 ||
        state.systemManagerLookupCalls != 2 ||
        state.imeInitialiseCalls != 1)
    {
        puts("FABLETLC_GFMAIN_PHASE9_BEHAVIOR FAIL code=3");
        return 3;
    }

    puts("FABLETLC_GFMAIN_PHASE9_BEHAVIOR PASS");
    return 0;
}
