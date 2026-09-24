#include "fable_ui_transform.h"
#pragma optimize("s", on)

// Semantic name inferred from coordinate conversion and CManager::GetUIScale;
// the former CTCLook label was propagated onto this unrelated global accessor.
float __cdecl FableUiGetCoordinateWidth()
{
    if (FableUiCoordinateConversionEnabled) return FableUiCoordinateDestinationExtent.x;
    return FableUiCoordinateSourceExtent.x;
}
