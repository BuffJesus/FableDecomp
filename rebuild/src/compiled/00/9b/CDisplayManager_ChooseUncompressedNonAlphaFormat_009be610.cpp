#include "fable_ui_display_formats.h"
bool __fastcall FableUiChooseUncompressedNonAlphaFormat(void* receiver,void*,int bits,int* out,bool renderTarget)
{
    FableUiPixelFormatDescription* chosen=0;
    for(FableUiPixelFormatDescription* row=FableUiPixelFormats;row->Bits!=-1;++row)
        if(row->Type==2 && row->Bits==bits && row->Red>0 && row->Green>0 && row->Blue>0 &&
           (!chosen || row->Alpha<chosen->Alpha) &&
           FableUiSupportsFormat(static_cast<FableUiDisplayFormatView*>(receiver),row->D3DFormat,renderTarget ? 1 : 0)) chosen=row;
    if(!chosen) return false;
    FableUiSetPixelFormat(out,0,chosen->D3DFormat); return true;
}
