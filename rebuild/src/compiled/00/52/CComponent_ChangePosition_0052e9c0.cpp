#include "fable_ui_transform.h"

void __fastcall FableUiChangePosition(FableUiComponentDrawView* component, void*, const FableUiStateVector2* target, float duration, bool)
{
    component->PositionTime = duration;
    component->TargetPosition.x = target->x;
    component->TargetPosition.y = target->y;
    if (duration > 0)
    {
        component->InitialPosition = component->Position;
        component->PositionTimeElapsed = 0;
    }
    else
    {
        component->InitialPosition.x = target->x;
        component->InitialPosition.y = target->y;
        component->Position.x = target->x;
        component->Position.y = target->y;
    }
}
