#include "fable_video_system.h"

namespace
{
    typedef long (FABLE_STDCALL *FableMediaControlStop)(
        void* mediaControl);

    struct FableMediaControlVTable
    {
        void* methods00[9];
        FableMediaControlStop stop24;
    };

    struct FableMediaControl
    {
        FableMediaControlVTable* vtable;
    };
}

bool CVideoSysRecoveredLayout::Stop()
{
    if (playbackState28 == 0)
        return false;

    FableMediaControl* const mediaControl =
        static_cast<FableMediaControl*>(mediaControl04);
    mediaControl->vtable->stop24(mediaControl);
    WaitForState(0);
    playbackState28 = CVideoSysPlaybackStopped;
    return true;
}
