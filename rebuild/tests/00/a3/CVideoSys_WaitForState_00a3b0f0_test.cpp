#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

namespace
{
    struct FakeMediaControl
    {
        void** vtable;
    };

    fable_u32 g_stateToReturn;
    unsigned int g_callsBeforeState;
    unsigned int g_getStateCalls;

    long FABLE_STDCALL GetState(
        void* mediaControl,
        long timeoutMilliseconds,
        fable_u32* state)
    {
        if (
            mediaControl == 0 ||
            timeoutMilliseconds != 10 ||
            state == 0)
        {
            return -1;
        }
        ++g_getStateCalls;
        if (g_getStateCalls >= g_callsBeforeState)
            *state = g_stateToReturn;
        else
            *state = 0xFFFFFFFF;
        return 0;
    }
}

int main()
{
    void* vtable[11];
    memset(vtable, 0, sizeof(vtable));
    vtable[10] = reinterpret_cast<void*>(&GetState);
    FakeMediaControl mediaControl = {vtable};

    CVideoSysRecoveredLayout videoSystem;
    memset(&videoSystem, 0, sizeof(videoSystem));
    videoSystem.mediaControl04 = &mediaControl;

    g_stateToReturn = 2;
    g_callsBeforeState = 3;
    videoSystem.WaitForState(2);
    if (g_getStateCalls != 3)
        return 1;

    g_getStateCalls = 0;
    g_stateToReturn = 1;
    g_callsBeforeState = 101;
    videoSystem.WaitForState(2);
    if (g_getStateCalls != 100)
        return 2;

    printf("FABLETLC_CVIDEOSYS_WAIT_FOR_STATE PASS\n");
    return 0;
}
