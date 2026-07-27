#include "fable_video_system.h"

namespace
{
    typedef long (FABLE_STDCALL *FableMediaControlRun)(
        void* mediaControl);

    struct FableMediaControlVTable
    {
        void* methods00[7];
        FableMediaControlRun run1C;
    };

    struct FableMediaControl
    {
        FableMediaControlVTable* vtable;
    };
}

bool CVideoSysRecoveredLayout::AttemptToPlay()
{
    FableMediaControl* const mediaControl =
        static_cast<FableMediaControl*>(mediaControl04);
    if (mediaControl->vtable->run1C(mediaControl) == 0)
    {
        playbackState28 = CVideoSysPlaybackPlaying;
        return true;
    }
    return false;
}
