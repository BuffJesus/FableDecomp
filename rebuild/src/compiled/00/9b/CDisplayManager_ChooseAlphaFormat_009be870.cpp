#include "fable_ui_display_formats.h"
bool __fastcall FableUiChooseAlphaFormat(void* receiver,void*,int,int* out)
{
    if(!FableUiSupportsFormat(static_cast<FableUiDisplayFormatView*>(receiver),0x33545844,0)) return false;
    FableUiSetPixelFormat(out,0,0x33545844); return true;
}
