#include <stdio.h>

#include "fable_boot.h"
#include "fable_cgame.h"

// Storage behind the CSystemManager singleton GFGetSystemManager returns;
// the engine boundary installs its stand-in managers there.
extern "C" fable_u32 g_testSystemManagerStorage[0x180 / 4] = {};
#pragma comment(linker, "/alternatename:?GSystemManager@@3VCSystemManager@@A=_g_testSystemManagerStorage")

// Stand-in for CInitBaseClass::Initialise (0x0099A350) so the harness can
// exercise CGame::Initialise's rejected branch; retail's base always succeeds.
bool CInitBaseClass::Initialise()
{
    ++g_FableProgressBeginCalls;
    return g_FableProgressBeginEnabled;
}

namespace
{
    // CGame viewed as bytes: CurrentGameComponent at +8, Quit at +0x20C.
    union
    {
        double align;
        unsigned char bytes[0x210];
    } g_gameStorage;

    fable_u32& CurrentComponentBits()
    {
        return *reinterpret_cast<fable_u32*>(g_gameStorage.bytes + 0x08);
    }

    unsigned char& QuitByte()
    {
        return g_gameStorage.bytes[0x20C];
    }

    void ResetFixture()
    {
        g_GFInitialiseProgressPhaseState =
            GFInitialiseProgressPhaseState();
        g_GFInitialiseProgressPhaseState.displayWidth = 1365;
        g_GFInitialiseProgressPhaseState.game =
            reinterpret_cast<CGame*>(g_gameStorage.bytes);
        CurrentComponentBits() = 0x11223344;
        QuitByte() = 0x7F;

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
        CurrentComponentBits() != 0 ||
        QuitByte() != 0 ||
        g_GFInitialiseProgressPhaseState.halfDisplayWidth != 682)
    {
        return 2;
    }
    if (
        g_FableProgressBeginCalls != 1 ||
        g_FableProgressConstructCalls != 1 ||
        g_FableProgressInstallCalls != 0 ||
        g_FableProgressDestroyCalls != 0 ||
        g_FableProgressDisplay_013CAA38.object == 0 ||
        g_FableProgressDisplay_013CAA38.reference == 0 ||
        g_FableProgressDisplay_013CAA38.reference->owners != 1)
    {
        return 3;
    }

    {
        CCountedProgressDisplay snapshot = GetProgressDisplay();
        if (
            snapshot.object != g_FableProgressDisplay_013CAA38.object ||
            snapshot.reference !=
                g_FableProgressDisplay_013CAA38.reference ||
            snapshot.reference->owners != 2 ||
            snapshot.object->IsActive())
        {
            return 8;
        }
    }
    if (g_FableProgressDisplay_013CAA38.reference->owners != 1)
        return 9;

    FableReleaseProgressDisplayBoundary();
    if (
        g_FableProgressDisplay_013CAA38.object != 0 ||
        g_FableProgressDisplay_013CAA38.reference != 0 ||
        g_FableProgressDestroyCalls != 1)
    {
        return 7;
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
        CurrentComponentBits() != 0x11223344 ||
        QuitByte() != 0x7F ||
        g_GFInitialiseProgressPhaseState.halfDisplayWidth != 0)
    {
        return 5;
    }
    if (
        g_FableProgressBeginCalls != 1 ||
        g_FableProgressConstructCalls != 0 ||
        g_FableProgressInstallCalls != 0 ||
        g_FableProgressDestroyCalls != 0 ||
        g_FableProgressDisplay_013CAA38.object != 0 ||
        g_FableProgressDisplay_013CAA38.reference != 0)
    {
        return 6;
    }

    printf("FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS\n");
    return 0;
}
