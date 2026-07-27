#include "fable_video_system.h"

namespace
{
    typedef long (FABLE_STDCALL *FableMediaControlGetState)(
        void* mediaControl,
        long timeoutMilliseconds,
        fable_u32* state);

    struct FableMediaControlVTable
    {
        void* methods00[10];
        FableMediaControlGetState getState28;
    };

    struct FableMediaControl
    {
        FableMediaControlVTable* vtable;
    };
}

void CVideoSysRecoveredLayout::WaitForState(
    fable_u32 desiredState)
{
    const fable_u32 expectedState = desiredState;
    int attempts = 0;
    do
    {
        FableMediaControl* const mediaControl =
            static_cast<FableMediaControl*>(mediaControl04);
        mediaControl->vtable->getState28(
            mediaControl,
            10,
            &desiredState);
        ++attempts;
        if (expectedState == desiredState)
            return;
    }
    while (attempts < 100);
}
