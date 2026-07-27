#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

namespace
{
    struct FakeMediaControl
    {
        void** vtable;
    };

    unsigned int g_stopCalls;
    unsigned int g_waitCalls;
    fable_u32 g_waitState;

    long FABLE_STDCALL Stop(void* mediaControl)
    {
        if (mediaControl == 0)
            return -1;
        ++g_stopCalls;
        return 0;
    }
}

void CVideoSysRecoveredLayout::WaitForState(
    fable_u32 desiredState)
{
    ++g_waitCalls;
    g_waitState = desiredState;
}

int main()
{
    void* vtable[10];
    memset(vtable, 0, sizeof(vtable));
    vtable[9] = reinterpret_cast<void*>(&Stop);
    FakeMediaControl mediaControl = {vtable};

    CVideoSysRecoveredLayout videoSystem;
    memset(&videoSystem, 0, sizeof(videoSystem));
    videoSystem.mediaControl04 = &mediaControl;

    if (
        videoSystem.Stop() ||
        g_stopCalls != 0 ||
        g_waitCalls != 0)
    {
        return 1;
    }

    videoSystem.playbackState28 = CVideoSysPlaybackPlaying;
    if (
        !videoSystem.Stop() ||
        g_stopCalls != 1 ||
        g_waitCalls != 1 ||
        g_waitState != 0 ||
        videoSystem.playbackState28 != CVideoSysPlaybackStopped)
    {
        return 2;
    }

    printf("FABLETLC_CVIDEOSYS_STOP PASS\n");
    return 0;
}
