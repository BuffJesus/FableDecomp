#include "fable_ui_transform.h"

FableUiStateVector2* __fastcall FableUiGetManagerScale(void*, void*, FableUiStateVector2* output)
{
    FableUiStateVector2 scale = {1.0f, 1.0f};
    if (FableUiScaleContext &&
        (FableUiGetCoordinateWidth() < 1024.0f || FableUiGetCoordinateHeight() < 768.0f))
    {
        scale.x = FableUiGetCoordinateWidth() * (1.0f / 1024.0f);
        scale.y = FableUiGetCoordinateHeight() * (1.0f / 768.0f);
    }
    *output = scale;
    return output;
}
