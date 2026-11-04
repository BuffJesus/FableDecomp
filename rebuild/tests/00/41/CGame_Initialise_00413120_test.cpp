#include <stdio.h>
#include <string.h>

#include "fable_boot.h"
#include "fable_cgame.h"

namespace
{
    unsigned char g_progressDisplayStorage[sizeof(CProgressDisplay)];
    CProgressDisplay* const g_progressDisplay =
        reinterpret_cast<CProgressDisplay*>(g_progressDisplayStorage);
    FableReferenceCount g_reference;
    bool g_baseResult = true;
    bool g_allocationEnabled = true;
    int g_baseCalls = 0;
    const void* g_baseReceiver = 0;
    int g_constructCalls = 0;
    int g_setCalls = 0;
    int g_destroyCalls = 0;
    int g_deleteCalls = 0;
    CProgressDisplay* g_observedDisplay = 0;

    // CGame viewed as bytes: CurrentGameComponent at +8, Quit at +0x20C.
    union GameStorage
    {
        double align;
        unsigned char bytes[sizeof(CGame)];
    };

    CGame* PrepareGame(GameStorage& storage)
    {
        memset(storage.bytes, 0x5C, sizeof(storage.bytes));
        *reinterpret_cast<fable_u32*>(storage.bytes + 0x08) = 0x11223344;
        storage.bytes[0x20C] = 0x7F;
        return reinterpret_cast<CGame*>(storage.bytes);
    }

    fable_u32 CurrentComponentBits(const GameStorage& storage)
    {
        return *reinterpret_cast<const fable_u32*>(storage.bytes + 0x08);
    }
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

bool CInitBaseClass::Initialise()
{
    ++g_baseCalls;
    g_baseReceiver = this;
    return g_baseResult;
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
    g_baseResult = true;
    g_allocationEnabled = true;
    g_baseCalls = 0;
    g_baseReceiver = 0;
    g_constructCalls = 0;
    g_setCalls = 0;
    g_destroyCalls = 0;
    g_deleteCalls = 0;
    g_observedDisplay = 0;
}

static int VerifyRejectedBase()
{
    ResetFixture();
    g_baseResult = false;

    GameStorage storage;
    CGame* game = PrepareGame(storage);
    if (game->Initialise())
        return 1;
    if (g_baseCalls != 1 || g_baseReceiver != storage.bytes)
        return 2;
    if (g_constructCalls != 0 || g_setCalls != 0)
        return 3;
    if (CurrentComponentBits(storage) != 0x11223344 || storage.bytes[0x20C] != 0x7F)
        return 4;
    return 0;
}

static int VerifySuccessfulInitialise()
{
    ResetFixture();

    GameStorage storage;
    CGame* game = PrepareGame(storage);
    if (!game->Initialise())
        return 5;
    if (CurrentComponentBits(storage) != 0 || storage.bytes[0x20C] != 0)
        return 6;
    // Only the two fields move; the base object and parameter buffer stay.
    if (storage.bytes[0] != 0x5C || storage.bytes[0x0C] != 0x5C || storage.bytes[0x20B] != 0x5C)
        return 7;
    if (g_baseCalls != 1 || g_baseReceiver != storage.bytes)
        return 8;
    if (g_constructCalls != 1 || g_setCalls != 1)
        return 9;
    if (g_observedDisplay != g_progressDisplay)
        return 10;
    // The local counted pointer releases its reference on the way out.
    if (g_destroyCalls != 1 || g_deleteCalls != 1)
        return 11;
    return 0;
}

static int VerifyAllocationFailureStillInstallsNull()
{
    ResetFixture();
    g_allocationEnabled = false;

    GameStorage storage;
    CGame* game = PrepareGame(storage);
    if (!game->Initialise())
        return 12;
    if (g_constructCalls != 0 || g_setCalls != 1)
        return 13;
    if (g_observedDisplay != 0 || g_destroyCalls != 0 || g_deleteCalls != 0)
        return 14;
    return 0;
}

int main()
{
    int result = VerifyRejectedBase();
    if (result == 0)
        result = VerifySuccessfulInitialise();
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
