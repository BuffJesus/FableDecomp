#include "fable_ui_texture_surfaces.h"
int __fastcall FableUiPixelFormatBits(const int* format,void*)
{ return FableUiPixelFormats[*format].Bits; }
