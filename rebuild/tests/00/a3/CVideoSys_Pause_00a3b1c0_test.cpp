#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

namespace
{
    struct FakeMediaControl
    {
        void** vtable;
    };

    unsigned int g_pauseCalls;
    unsigned int g_waitCalls;
    fable_u32 g_waitState;

    long FABLE_STDCALL Pause(void* mediaControl)
    {
        if (mediaControl == 0)
            return -1;
        ++g_pauseCalls;
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
    void* vtable[9];
    memset(vtable, 0, sizeof(vtable));
    vtable[8] = reinterpret_cast<void*>(&Pause);
    FakeMediaControl mediaControl = {vtable};

    CVideoSysRecoveredLayout videoSystem;
    memset(&videoSystem, 0, sizeof(videoSystem));
    videoSystem.mediaControl04 = &mediaControl;

    if (
        videoSystem.Pause() ||
        g_pauseCalls != 0 ||
        g_waitCalls != 0)
    {
        return 1;
    }

    videoSystem.playbackState28 = CVideoSysPlaybackPlaying;
    if (
        !videoSystem.Pause() ||
        g_pauseCalls != 1 ||
        g_waitCalls != 1 ||
        g_waitState != 1 ||
        videoSystem.playbackState28 != CVideoSysPlaybackPaused)
    {
        return 2;
    }

    printf("FABLETLC_CVIDEOSYS_PAUSE PASS\n");
    return 0;
}
