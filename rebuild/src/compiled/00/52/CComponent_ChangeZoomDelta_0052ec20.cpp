#include "fable_ui_transform.h"

void __fastcall FableUiChangeZoomDelta(FableUiComponentDrawView* component, void*, const FableUiStateVector2* delta, float duration, bool linear)
{
    FableUiStateVector2 target = {component->Zoom.x + delta->x, component->Zoom.y + delta->y};
    component->Vtable->ChangeZoom(component, 0, &target, duration, linear);
}
