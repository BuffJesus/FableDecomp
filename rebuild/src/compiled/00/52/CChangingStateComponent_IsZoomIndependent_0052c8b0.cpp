#include "fable_ui_state_progress.h"

bool __fastcall FableUiStateZoomIndependent(FableUiComponentDrawView* view, void*)
{
    const bool independent = FableUiBaseZoomIndependent(view, 0);
    FableUiStateProgressView* component = static_cast<FableUiStateProgressView*>(view);
    const CUIStateRecoveredLayout* state = component->Vtable->FindStateForQuery(component, 0, component->TargetState);
    return independent || (state && (state->stateChangeFlag & 0x40) != 0);
}
