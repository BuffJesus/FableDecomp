#include "fable_progress_display.h"
void FABLE_FASTCALL NProgressDisplay::StopProgress(bool a,bool b) {
    if(PProgressDisplay.Get())PProgressDisplay->StopProgress(a,b);
}
