#include "fable_ui_transform.h"

FableUiStateVector2* __fastcall FableUiConvertCoordinatesInverse(FableUiStateVector2* output, void*, FableUiStateVector2 value)
{
    if (FableUiCoordinateConversionEnabled)
    {
        value.x = value.x / FableUiCoordinateDestinationExtent.x * FableUiCoordinateSourceExtent.x;
        value.y = value.y / FableUiCoordinateDestinationExtent.y * FableUiCoordinateSourceExtent.y;
    }
    *output = value;
    return output;
}
