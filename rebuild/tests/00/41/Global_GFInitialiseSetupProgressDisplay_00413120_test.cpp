#include <stdio.h>

#include "fable_boot.h"

namespace
{
    unsigned char g_progressDisplayStorage[sizeof(CProgressDisplay)];
    CProgressDisplay* const g_progressDisplay =
        reinterpret_cast<CProgressDisplay*>(g_progressDisplayStorage);
    FableReferenceCount g_reference;
    fable_u8 g_beginResult = 1;
    bool g_allocationEnabled = true;
    int g_beginCalls = 0;
    int g_constructCalls = 0;
    int g_setCalls = 0;
    int g_destroyCalls = 0;
    int g_deleteCalls = 0;
    CProgressDisplay* g_observedDisplay = 0;
}

CWideString::CWideString()
{
}

CCharString::CCharString()
{
}

void* FABLE_CDECL FableOperatorNew(fable_u32 size)
{
    if (!g_allocationEnabled || size != 0x88)
        return 0;
    return g_progressDisplay;
}

void FABLE_CDECL FableOperatorDelete(void* allocation)
{
    if (allocation == &g_reference)
        ++g_deleteCalls;
}

static void FABLE_FASTCALL DestroyProgressDisplay(void* object)
{
    if (object == g_progressDisplay)
        ++g_destroyCalls;
}

CCountedProgressDisplay::CCountedProgressDisplay(CProgressDisplay* value)
    : object(value),
      reference(0)
{
    if (value != 0)
    {
        g_reference.owners = 1;
        g_reference.destroy = DestroyProgressDisplay;
        g_reference.object = value;
        reference = &g_reference;
    }
}

fable_u8 FABLE_FASTCALL GFInitialiseState_Begin(GFInitialiseState* /* state */)
{
    ++g_beginCalls;
    return g_beginResult;
}

CProgressDisplay::CProgressDisplay()
{
    ++g_constructCalls;
}

void FABLE_FASTCALL SetProgressDisplay(CCountedProgressDisplay* display)
{
    ++g_setCalls;
    g_observedDisplay = display->object;
}

static void ResetFixture()
{
    g_beginResult = 1;
    g_allocationEnabled = true;
    g_beginCalls = 0;
    g_constructCalls = 0;
    g_setCalls = 0;
    g_destroyCalls = 0;
    g_deleteCalls = 0;
    g_observedDisplay = 0;
}

static int VerifyRejectedBegin()
{
    ResetFixture();
    g_beginResult = 0;

    GFInitialiseState state = {};
    state.startupValue08 = 0x11223344;
    state.startupFlag20C = 0x7F;

    if (GFInitialise_SetupProgressDisplay(&state) != 0)
        return 1;
    if (g_beginCalls != 1 || g_constructCalls != 0 || g_setCalls != 0)
        return 2;
    if (state.startupValue08 != 0x11223344 || state.startupFlag20C != 0x7F)
        return 3;
    return 0;
}

static int VerifySuccessfulSetup()
{
    ResetFixture();

    GFInitialiseState state = {};
    state.startupValue08 = 0x11223344;
    state.startupFlag20C = 0x7F;

    if (GFInitialise_SetupProgressDisplay(&state) != 1)
        return 4;
    if (state.startupValue08 != 0 || state.startupFlag20C != 0)
        return 5;
    if (g_beginCalls != 1 || g_constructCalls != 1 || g_setCalls != 1)
        return 6;
    if (g_observedDisplay != g_progressDisplay)
        return 7;
    if (g_destroyCalls != 1 || g_deleteCalls != 1)
        return 8;
    return 0;
}

static int VerifyAllocationFailureStillInstallsNull()
{
    ResetFixture();
    g_allocationEnabled = false;

    GFInitialiseState state = {};
    if (GFInitialise_SetupProgressDisplay(&state) != 1)
        return 9;
    if (g_constructCalls != 0 || g_setCalls != 1)
        return 10;
    if (g_observedDisplay != 0 || g_destroyCalls != 0 || g_deleteCalls != 0)
        return 11;
    return 0;
}

int main()
{
    int result = VerifyRejectedBegin();
    if (result == 0)
        result = VerifySuccessfulSetup();
    if (result == 0)
        result = VerifyAllocationFailureStillInstallsNull();

    if (result != 0)
    {
        printf("FABLETLC_PROGRESS_SETUP_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS\n");
    return 0;
}
