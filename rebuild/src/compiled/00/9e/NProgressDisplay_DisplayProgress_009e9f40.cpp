// NProgressDisplay::DisplayProgress, retail 0x009E9F40
// (FableWin ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z):
// forward to the installed progress display, if any.
#include "fable_progress_display.h"

void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString& text, float fraction, bool forceRender, bool isDebug)
{
    if (PProgressDisplay.Get())
        PProgressDisplay->DisplayProgress(text, fraction, forceRender, isDebug);
}