#include "fable_ui_display_formats.h"
bool __fastcall FableUiChooseUncompressedAlphaFormat(void* receiver,void*,int bits,int* out)
{
    for(FableUiPixelFormatDescription* row=FableUiPixelFormats;row->Bits!=-1;++row)
        if(row->Type==2 && row->Bits==bits && row->Red>1 && row->Green>1 && row->Blue>1 && row->Alpha>1 &&
            FableUiSupportsFormat(static_cast<FableUiDisplayFormatView*>(receiver),row->D3DFormat,0))
        { FableUiSetPixelFormat(out,0,row->D3DFormat); return true; }
    return false;
}
