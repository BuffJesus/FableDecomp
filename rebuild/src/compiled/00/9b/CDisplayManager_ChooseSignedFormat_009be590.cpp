#include "fable_ui_display_formats.h"
bool __fastcall FableUiChooseSignedFormat(void* receiver,void*,int bits,int* out)
{
    FableUiPixelFormatDescription* chosen=0;
    for(FableUiPixelFormatDescription* row=FableUiPixelFormats;row->Bits!=-1;++row)
        if(row->Type==6 && row->Bits==bits && !chosen && FableUiSupportsFormat(static_cast<FableUiDisplayFormatView*>(receiver),row->D3DFormat,0)) chosen=row;
    if(!chosen) return false;
    FableUiSetPixelFormat(out,0,chosen->D3DFormat); return true;
}
