#include "fable_ui_transform.h"
#include <stdio.h>
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent={640,480},FableUiCoordinateDestinationExtent={1920,1080};
int main()
{
    if(FableUiGetCoordinateHeight()!=480) return 1;
    FableUiCoordinateConversionEnabled=255;
    if(FableUiGetCoordinateHeight()!=1080) return 2;
    puts("UI_DIMENSION_HEIGHT PASS"); return 0;
}
