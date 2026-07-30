#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "fable_gfmain.h"

namespace
{
    void ResetState(GFMainPhase7State& state)
    {
        memset(&state, 0, sizeof(state));
        state.staticMapWorkEnabled = true;
        state.language = 0;
        wcscpy(
            state.windowTitleSource,
            L"Fable - The Lost Chapters");
    }

    bool HasDefaultStrings(const GFMainPhase7State& state)
    {
        return
            wcscmp(
                state.shaderResourcePath,
                L"Data\\Shaders\\") == 0 &&
            wcscmp(
                state.windowTitle,
                L"Fable - The Lost Chapters") == 0 &&
            wcscmp(
                state.cacheDirectory,
                L"Data\\Cache\\") == 0 &&
            strcmp(
                state.streamingFontBank,
                "STREAMING_FONT_ENGLISH_PC") == 0;
    }
}

int main()
{
    GFMainPhase7State state;
    ResetState(state);
    state.instanceHandleToken = 0x12345678UL;
    strcpy(state.commandLine, "-q 7");
    state.windowsShow = 3;
    state.preferPrimaryDisplayDevice = true;
    state.regionWords[0] = 11;
    state.regionWords[1] = 12;
    state.regionWords[2] = 13;
    state.regionWords[3] = 14;
    state.regionFlag18 = true;
    state.regionWord20 = 21;
    state.regionWord24 = 22;
    state.regionFlag28 = true;
    state.exclusiveMode = true;
    state.skipConfigDetection = true;

    if (
        FableRunGFMainPhase7(state) != 0 ||
        state.configuredInstanceHandleToken != 0x12345678UL ||
        strcmp(state.configuredCommandLine, "-q 7") != 0 ||
        state.configuredWindowsShow != 3 ||
        !state.configuredPreferPrimaryDisplayDevice ||
        state.startupFlags != 5 ||
        state.displayDepth != 64.0f ||
        state.nearPlane != 2.0f ||
        state.displayScale != 1.0f ||
        state.displayUnknown4C != 0 ||
        state.memoryBudget != 0x00040000UL ||
        state.configuredRegionWords[0] != 11 ||
        state.configuredRegionWords[3] != 14 ||
        !state.configuredRegionFlag18 ||
        state.configuredRegionWord20 != 21 ||
        state.configuredRegionWord24 != 22 ||
        !state.configuredRegionFlag28 ||
        !state.configuredRegionFlag10 ||
        !state.configuredRegionFlag19 ||
        !state.configuredUnknown58 ||
        !state.configuredUnknown59 ||
        !state.configuredUnknown5A ||
        state.configuredRegionMode != 0 ||
        !state.errorCallbackInstalled ||
        !state.configuredExclusiveMode ||
        !state.configuredWaitWhileInactive ||
        !state.configuredSkipConfigDetection ||
        state.skipPhase8 ||
        state.windowTitleCalls != 1 ||
        state.exactWindowTitleCoordinatorCalls != 0 ||
        state.exactWindowTitleBankOpenCalls != 0 ||
        state.exactWindowTitleLookupCalls != 0 ||
        state.exactWindowTitleBankDeleteCalls != 0 ||
        !state.staticMapWorkEnabled ||
        !HasDefaultStrings(state))
    {
        puts("FABLETLC_GFMAIN_PHASE7_BEHAVIOR FAIL code=1");
        return 1;
    }

    ResetState(state);
    state.useAlternateDisplayDefaults = true;
    state.overrideRegionMode = true;
    state.alternateRegionMode = false;
    state.disableInactiveWait = true;
    state.language = 16;

    if (
        FableRunGFMainPhase7(state) != 0 ||
        state.startupFlags != 0x15 ||
        state.displayDepth != 32.0f ||
        state.nearPlane != 5.0f ||
        state.displayScale != 1.0f ||
        state.displayUnknown4C != 0x100 ||
        state.memoryBudget != 0x00010000UL ||
        state.configuredRegionMode != 4 ||
        state.configuredWaitWhileInactive ||
        strcmp(
            state.streamingFontBank,
            "STREAMING_FONT_FRENCH_PC") != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE7_BEHAVIOR FAIL code=2");
        return 2;
    }

    ResetState(state);
    state.overrideRegionMode = true;
    state.alternateRegionMode = true;
    state.staticMapMode = true;
    state.language = 18;

    if (
        FableRunGFMainPhase7(state) != 0 ||
        state.configuredRegionMode != 9 ||
        !state.skipPhase8 ||
        !state.configuredSkipConfigDetection ||
        state.staticMapWorkEnabled ||
        strcmp(
            state.streamingFontBank,
            "STREAMING_FONT_KOREAN_PC") != 0)
    {
        puts("FABLETLC_GFMAIN_PHASE7_BEHAVIOR FAIL code=3");
        return 3;
    }

    puts("FABLETLC_GFMAIN_PHASE7_BEHAVIOR PASS");
    return 0;
}
