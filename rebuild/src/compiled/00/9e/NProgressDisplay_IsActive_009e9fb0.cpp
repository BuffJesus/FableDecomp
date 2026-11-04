// NProgressDisplay::IsActive, retail 0x009E9FB0 (FableWin ?IsActive@NProgressDisplay@@YI_NXZ).
#include "fable_progress_display.h"

bool FABLE_FASTCALL NProgressDisplay::IsActive()
{
    if (PProgressDisplay.Get())
        return PProgressDisplay->IsActive();
    return false;
}