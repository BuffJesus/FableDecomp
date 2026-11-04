// NProgressDisplay::SetProgressDisplay, retail 0x009E9FD0
// (Ego_r ?SetProgressDisplay@NProgressDisplay@@YIXAAV?$CCountedPointer@VCAProgressDisplay@@@@@Z):
// drop the installed display, then share the caller's (FableWin 0x02FB3830:
// IsNull, reset(0), operator=).
#include "fable_progress_display.h"
#include "fable_counted_pointer_inline.h"
#include "fable_counted_pointer_reset_inline.h"

namespace NProgressDisplay
{
    void FABLE_FASTCALL SetProgressDisplay(CCountedPointer<CAProgressDisplay>& display);
}

void FABLE_FASTCALL NProgressDisplay::SetProgressDisplay(CCountedPointer<CAProgressDisplay>& display)
{
    if (!PProgressDisplay.IsNull())
        PProgressDisplay.reset(0);
    PProgressDisplay = display;
}
