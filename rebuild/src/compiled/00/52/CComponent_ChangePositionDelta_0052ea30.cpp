#include "fable_ui_transform.h"

void __fastcall FableUiChangePositionDelta(FableUiComponentDrawView* component, void*, const FableUiStateVector2* delta, float duration, bool linear)
{
    FableUiStateVector2 local = *delta;
    double vertical = local.y;
    if (component->Vtable->UseRelativePosition(component, 0))
    {
        FableUiConvertCoordinatesInverse(&local, 0, local);
        FableUiStateVector2 scale;
        FableUiGetManagerScale(FableUiGetManager(), 0, &scale);
        local.x *= scale.x;
        FableUiGetManagerScale(FableUiGetManager(), 0, &scale);
        vertical = static_cast<double>(local.y) * scale.y;
    }
    FableUiStateVector2 target = {local.x + component->Position.x,
        static_cast<float>(vertical + component->Position.y)};
    component->Vtable->ChangePosition(component, 0, &target, duration, linear);
}
