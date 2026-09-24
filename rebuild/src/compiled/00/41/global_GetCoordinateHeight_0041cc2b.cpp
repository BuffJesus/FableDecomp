#include "fable_ui_transform.h"
#pragma optimize("s", on)

float __cdecl FableUiGetCoordinateHeight()
{
    if (FableUiCoordinateConversionEnabled) return FableUiCoordinateDestinationExtent.y;
    return FableUiCoordinateSourceExtent.y;
}
