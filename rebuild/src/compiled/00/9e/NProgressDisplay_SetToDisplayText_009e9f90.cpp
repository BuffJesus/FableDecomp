// NProgressDisplay::SetToDisplayText, retail 0x009E9F90
// (FableWin ?SetToDisplayText@NProgressDisplay@@YIX_N@Z): forward to the
// installed progress display, if any.
#include "fable_progress_display.h"

void FABLE_FASTCALL NProgressDisplay::SetToDisplayText(bool displayText)
{
    if (PProgressDisplay.Get())
        PProgressDisplay->SetToDisplayText(displayText);
}