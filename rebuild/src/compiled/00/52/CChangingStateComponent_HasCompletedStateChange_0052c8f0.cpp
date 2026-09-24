#include "fable_ui_state_progress.h"

bool __fastcall FableUiHasCompletedStateChange(FableUiComponentDrawView* view, void*)
{
    const FableUiStateProgressView* component = static_cast<FableUiStateProgressView*>(view);
    return component->StatesToDo->Next == component->StatesToDo &&
        (component->StatesDone & component->StatesBeingDone) == component->StatesBeingDone;
}
