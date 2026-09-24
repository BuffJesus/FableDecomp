#include "fable_ui_display_services.h"
#include <stdio.h>
unsigned char FableUiCoordinateConversionEnabled=0;
FableUiStateVector2 FableUiCoordinateDestinationExtent={0,0};
static FableUiSystemManagerView systemManager;
static unsigned calls;
FableUiSystemManagerView* __cdecl FableUiGetSystemManager() { return &systemManager; }
void __fastcall FableUiQueryDisplayExtent(void*,void*,FableUiDisplayExtent* out)
{ ++calls; out->Width=1920; out->Height=1080; }
int main()
{
    FableUiSetRelativeCoordinates(true); FableUiSetRelativeCoordinates(true); FableUiSetRelativeCoordinates(false);
    if(calls!=1 || FableUiCoordinateConversionEnabled || FableUiCoordinateDestinationExtent.x!=1920 || FableUiCoordinateDestinationExtent.y!=1080) return 1;
    puts("UI_COORDINATE_SETUP_CANDIDATE PASS"); return 0;
}
