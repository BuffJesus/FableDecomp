#include "fable_video_system.h"

namespace
{
    typedef long (FABLE_STDCALL *FableMediaControlPause)(
        void* mediaControl);

    struct FableMediaControlVTable
    {
        void* methods00[8];
        FableMediaControlPause pause20;
    };

    struct FableMediaControl
    {
        FableMediaControlVTable* vtable;
    };
}

bool CVideoSysRecoveredLayout::Pause()
{
    if (playbackState28 == 0)
        return false;

    FableMediaControl* const mediaControl =
        static_cast<FableMediaControl*>(mediaControl04);
    mediaControl->vtable->pause20(mediaControl);
    WaitForState(1);
    playbackState28 = CVideoSysPlaybackPaused;
    return true;
}
