#include "fable_ui_display_formats.h"
void __fastcall FableUiSetPixelFormat(int* out,void*,unsigned format)
{
    // Retail compares the first entry even if its Bits is the sentinel value.
    int i=0;
    do { if(FableUiPixelFormats[i].D3DFormat==format) { *out=i; return; } ++i; }
    while(FableUiPixelFormats[i].Bits!=-1);
    *out=-1;
}
