// NProgressDisplay::StartProgress, retail 0x009E9EF0
// (FableWin ?StartProgress@NProgressDisplay@@YIX_N00000@Z): forward to the
// installed progress display, if any.
#include "fable_progress_display.h"

void FABLE_FASTCALL NProgressDisplay::StartProgress(bool flag0, bool flag1, bool flag2, bool flag3, bool flag4, bool flag5)
{
    if (PProgressDisplay.Get())
        PProgressDisplay->StartProgress(flag0, flag1, flag2, flag3, flag4, flag5);
}