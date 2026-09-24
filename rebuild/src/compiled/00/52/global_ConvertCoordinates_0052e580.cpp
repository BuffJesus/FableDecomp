#include "fable_ui_transform.h"

FableUiStateVector2* __fastcall FableUiConvertCoordinates(FableUiStateVector2* output, void*, FableUiStateVector2 value)
{
    if (FableUiCoordinateConversionEnabled)
    {
        value.x = value.x / FableUiCoordinateSourceExtent.x * FableUiCoordinateDestinationExtent.x;
        value.y = value.y / FableUiCoordinateSourceExtent.y * FableUiCoordinateDestinationExtent.y;
    }
    *output = value;
    return output;
}
