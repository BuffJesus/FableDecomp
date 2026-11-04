// NProgressDisplay::GetProgressDisplay, retail 0x009EA060
// (Ego_r ?GetProgressDisplay@NProgressDisplay@@YI?AV?$CCountedPointer@VCAProgressDisplay@@@@XZ):
// a shared copy of the installed display.
#include "fable_progress_display.h"
#include "fable_counted_pointer_inline.h"

namespace NProgressDisplay
{
    CCountedPointer<CAProgressDisplay> FABLE_FASTCALL GetProgressDisplay();
}

CCountedPointer<CAProgressDisplay> FABLE_FASTCALL NProgressDisplay::GetProgressDisplay()
{
    return PProgressDisplay;
}
