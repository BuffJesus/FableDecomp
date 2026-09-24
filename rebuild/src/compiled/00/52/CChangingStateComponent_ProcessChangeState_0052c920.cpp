#include "fable_ui_state_progress.h"

static bool DifferentVector(const FableUiStateVector2& target, const FableUiStateVector2& current)
{
    // Retail compares the squared distance to squared float 0.0001, retaining
    // x87 precision for the subtraction/products until the comparison.
    const double x = static_cast<double>(target.x) - current.x;
    const double y = static_cast<double>(target.y) - current.y;
    const double epsilon = static_cast<double>(0.0001f);
    return x*x + y*y > epsilon*epsilon;
}

void __fastcall FableUiProcessChangeState(FableUiStateProgressView* component, void*)
{
    if (component->CurrentState == component->TargetState) return;
    CUIStateRecoveredLayout* state = component->Vtable->FindState(component, 0, component->TargetState);
    // The current-state lookup is observable even though its result is unused.
    component->Vtable->FindState(component, 0, component->CurrentState);
    if ((state->stateChangeFlag & 1) && DifferentVector(state->position, component->Position))
    {
        FableUiStateVector2 delta = {state->position.x - component->Position.x,
                                    state->position.y - component->Position.y};
        component->Vtable->ChangePositionDelta(component, 0, &delta, component->UpdateTime, state->linearChange != 0);
    }
    if ((state->stateChangeFlag & 4) && DifferentVector(state->zoom, component->Zoom))
    {
        FableUiStateVector2 delta = {state->zoom.x - component->Zoom.x,
                                    state->zoom.y - component->Zoom.y};
        component->Vtable->ChangeZoomDelta(component, 0, &delta, component->UpdateTime, state->linearChange != 0);
    }
    if (state->stateChangeFlag & 2)
    {
        // Colour deltas wrap modulo 256, including negative differences.
        FableUiStateColour delta = {
            static_cast<fable_u8>(state->colour.red - component->Colour.red),
            static_cast<fable_u8>(state->colour.green - component->Colour.green),
            static_cast<fable_u8>(state->colour.blue - component->Colour.blue),
            static_cast<fable_u8>(state->colour.alpha - component->Colour.alpha)};
        if (delta.red || delta.green || delta.blue || delta.alpha)
            component->Vtable->ChangeColour(component, 0, &delta, component->UpdateTime, state->linearChange != 0);
    }
}
