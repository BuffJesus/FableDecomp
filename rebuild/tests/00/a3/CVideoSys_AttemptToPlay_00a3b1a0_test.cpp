#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

namespace
{
    struct FakeMediaControl
    {
        void** vtable;
    };

    long g_runResult;
    unsigned int g_runCalls;

    long FABLE_STDCALL Run(void* mediaControl)
    {
        if (mediaControl == 0)
            return -1;
        ++g_runCalls;
        return g_runResult;
    }
}

int main()
{
    void* vtable[8];
    memset(vtable, 0, sizeof(vtable));
    vtable[7] = reinterpret_cast<void*>(&Run);
    FakeMediaControl mediaControl = {vtable};

    CVideoSysRecoveredLayout videoSystem;
    memset(&videoSystem, 0, sizeof(videoSystem));
    videoSystem.mediaControl04 = &mediaControl;
    videoSystem.playbackState28 = CVideoSysPlaybackStopped;

    g_runResult = 1;
    if (
        videoSystem.AttemptToPlay() ||
        videoSystem.playbackState28 != CVideoSysPlaybackStopped ||
        g_runCalls != 1)
    {
        return 1;
    }

    g_runResult = 0;
    if (
        !videoSystem.AttemptToPlay() ||
        videoSystem.playbackState28 != CVideoSysPlaybackPlaying ||
        g_runCalls != 2)
    {
        return 2;
    }

    printf("FABLETLC_CVIDEOSYS_ATTEMPT_TO_PLAY PASS\n");
    return 0;
}
