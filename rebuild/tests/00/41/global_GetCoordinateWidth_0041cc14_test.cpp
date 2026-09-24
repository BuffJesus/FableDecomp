#include "fable_ui_transform.h"
#include <stdio.h>
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent={640,480},FableUiCoordinateDestinationExtent={1920,1080};
int main()
{
    if(FableUiGetCoordinateWidth()!=640) return 1;
    FableUiCoordinateConversionEnabled=255;
    if(FableUiGetCoordinateWidth()!=1920) return 2;
    puts("UI_DIMENSION_WIDTH PASS"); return 0;
}
