#include "fable_ui_display_formats.h"
bool __fastcall FableUiChooseBooleanAlphaFormat(void* receiver,void*,int,int* out)
{
    if(!FableUiSupportsFormat(static_cast<FableUiDisplayFormatView*>(receiver),0x31545844,0)) return false;
    FableUiSetPixelFormat(out,0,0x31545844); return true;
}
