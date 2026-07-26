#include <stdio.h>

#include "fable_boot.h"

namespace
{
    void ResetFixture()
    {
        g_GFInitialiseProgressPhaseState =
            GFInitialiseProgressPhaseState();
        g_GFInitialiseProgressPhaseState.displayWidth = 1365;
        g_GFInitialiseProgressPhaseState.progressDisplay.startupValue08 =
            0x11223344;
        g_GFInitialiseProgressPhaseState.progressDisplay.startupFlag20C =
            0x7F;

        g_FableProgressBeginEnabled = true;
        g_FableProgressBeginCalls = 0;
        g_FableProgressConstructCalls = 0;
        g_FableProgressInstallCalls = 0;
        g_FableProgressDestroyCalls = 0;
    }
}

int main()
{
    ResetFixture();

    if (
        FableRunGFInitialiseProgressPhase(
            g_GFInitialiseProgressPhaseState) != 1)
    {
        return 1;
    }
    if (
        g_GFInitialiseProgressPhaseState.progressDisplay.startupValue08 != 0 ||
        g_GFInitialiseProgressPhaseState.progressDisplay.startupFlag20C != 0 ||
        g_GFInitialiseProgressPhaseState.halfDisplayWidth != 682)
    {
        return 2;
    }
    if (
        g_FableProgressBeginCalls != 1 ||
        g_FableProgressConstructCalls != 1 ||
        g_FableProgressInstallCalls != 1 ||
        g_FableProgressDestroyCalls != 1)
    {
        return 3;
    }

    ResetFixture();
    g_FableProgressBeginEnabled = false;
    if (
        FableRunGFInitialiseProgressPhase(
            g_GFInitialiseProgressPhaseState) != 0)
    {
        return 4;
    }
    if (
        g_GFInitialiseProgressPhaseState.progressDisplay.startupValue08 !=
            0x11223344 ||
        g_GFInitialiseProgressPhaseState.progressDisplay.startupFlag20C !=
            0x7F ||
        g_GFInitialiseProgressPhaseState.halfDisplayWidth != 0)
    {
        return 5;
    }
    if (
        g_FableProgressBeginCalls != 1 ||
        g_FableProgressConstructCalls != 0 ||
        g_FableProgressInstallCalls != 0 ||
        g_FableProgressDestroyCalls != 0)
    {
        return 6;
    }

    printf("FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS\n");
    return 0;
}
