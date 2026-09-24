#include "fable_ui_transform.h"

void __fastcall FableUiChangeZoom(FableUiComponentDrawView* component, void*, const FableUiStateVector2* target, float duration, bool)
{
    component->ZoomTime = duration;
    component->TargetZoom.x = target->x;
    component->TargetZoom.y = target->y;
    if (duration > 0)
    {
        component->InitialZoom = component->Zoom;
        component->ZoomTimeElapsed = 0;
    }
    else
    {
        component->InitialZoom.x = target->x;
        component->InitialZoom.y = target->y;
        component->Zoom.x = target->x;
        component->Zoom.y = target->y;
    }
}
