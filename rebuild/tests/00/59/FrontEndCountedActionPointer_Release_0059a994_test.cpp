#include <cstdio>

#include "../../../src/compiled/00/59/FrontEndCountedActionPointer_Release_0059a994.cpp"

static int g_DestroyCalls;
static int g_DeleteCalls;
static int g_Sequence;
static int g_DestroySequence;
static int g_DeleteSequence;
static void* g_DestroyedAction;
static void* g_DeletedInfo;

extern "C" void __fastcall
ProbeDestroy_0059a994(void* action, void*)
{
    ++g_DestroyCalls;
    g_DestroyedAction = action;
    g_DestroySequence = ++g_Sequence;
}

extern "C" void __cdecl
FrontEndCountedPointerDelete_0059a994(void* allocation)
{
    ++g_DeleteCalls;
    g_DeletedInfo = allocation;
    g_DeleteSequence = ++g_Sequence;
}

static void ResetProbe()
{
    g_DestroyCalls = 0;
    g_DeleteCalls = 0;
    g_Sequence = 0;
    g_DestroySequence = 0;
    g_DeleteSequence = 0;
    g_DestroyedAction = 0;
    g_DeletedInfo = 0;
}

int main()
{
    int failures = 0;
    int action = 0x1234;

    ResetProbe();
    FrontEndCountedActionPointer_0059a994 null_info;
    null_info.action = &action;
    null_info.pointer_info = 0;
    null_info.Release();
    if (null_info.action != 0 ||
        null_info.pointer_info != 0 ||
        g_DestroyCalls != 0 ||
        g_DeleteCalls != 0)
    {
        std::printf("null-info release failed\n");
        ++failures;
    }

    ResetProbe();
    FrontEndActionPointerInfo_0059a994 shared_info;
    shared_info.reference_count = 2;
    shared_info.destroy_callback = (void*)&ProbeDestroy_0059a994;
    shared_info.action = &action;

    FrontEndCountedActionPointer_0059a994 shared;
    shared.action = &action;
    shared.pointer_info = &shared_info;
    shared.Release();
    if (shared_info.reference_count != 1 ||
        shared.action != 0 ||
        shared.pointer_info != 0 ||
        g_DestroyCalls != 0 ||
        g_DeleteCalls != 0)
    {
        std::printf("shared-owner release failed\n");
        ++failures;
    }

    ResetProbe();
    FrontEndActionPointerInfo_0059a994 sole_info;
    sole_info.reference_count = 1;
    sole_info.destroy_callback = (void*)&ProbeDestroy_0059a994;
    sole_info.action = &action;

    FrontEndCountedActionPointer_0059a994 sole;
    sole.action = &action;
    sole.pointer_info = &sole_info;
    sole.Release();
    if (sole_info.reference_count != 0 ||
        sole.action != 0 ||
        sole.pointer_info != 0 ||
        g_DestroyCalls != 1 ||
        g_DeleteCalls != 1 ||
        g_DestroyedAction != &action ||
        g_DeletedInfo != &sole_info ||
        g_DestroySequence != 1 ||
        g_DeleteSequence != 2)
    {
        std::printf("last-owner release failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a994_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a994_TEST PASS\n");
    return 0;
}
