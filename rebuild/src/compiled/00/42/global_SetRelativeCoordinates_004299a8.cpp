#include "fable_ui_display_services.h"
void __fastcall FableUiSetRelativeCoordinates(bool enabled)
{
    if(FableUiCoordinateConversionEnabled==enabled) return;
    FableUiCoordinateConversionEnabled=enabled;
    if(enabled)
    {
        FableUiDisplayExtent extent;
        FableUiQueryDisplayExtent(FableUiGetSystemManager()->Display,0,&extent);
        FableUiCoordinateDestinationExtent.x=static_cast<float>(extent.Width);
        FableUiCoordinateDestinationExtent.y=static_cast<float>(extent.Height);
    }
}
