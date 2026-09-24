#include "fable_ui_state_progress.h"

void __fastcall FableUiChangingStateUpdate(FableUiStateProgressView* component, void*, float delta)
{
    component->PreviousUpdateChanged = component->Vtable->HasCompletedStateChange(component, 0);
    CUIStateRecoveredLayout* state = component->Vtable->FindState(component, 0, component->TargetState);
    if (state)
    {
        if (state->stateChangeFlag & 0x10)
        {
            component->ParentColour.red = component->ParentColour.green =
                component->ParentColour.blue = component->ParentColour.alpha = 255;
        }
        if (state->stateChangeFlag & 0x20)
            component->ParentPosition.x = component->ParentPosition.y = 0.0f;
        if (state->stateChangeFlag & 0x40)
            component->ParentZoom.x = component->ParentZoom.y = 1.0f;
    }
    FableUiBaseComponentUpdate(component, 0, delta);
    if (!component->PreviousUpdateChanged)
        component->Vtable->UpdateStateChange(component, 0);
}
