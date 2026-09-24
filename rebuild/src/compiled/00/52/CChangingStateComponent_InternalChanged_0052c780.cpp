#include "fable_ui_state_progress.h"

bool __fastcall FableUiInternalChanged(FableUiComponentDrawView* view, void*)
{
    FableUiStateProgressView* component = static_cast<FableUiStateProgressView*>(view);
    component->Vtable->FindState(component, 0, component->TargetState);
    return component->PositionTimeElapsed >= component->PositionTime &&
        component->ZoomTimeElapsed >= component->ZoomTime &&
        component->ColourTimeElapsed >= component->ColourTime;
}
