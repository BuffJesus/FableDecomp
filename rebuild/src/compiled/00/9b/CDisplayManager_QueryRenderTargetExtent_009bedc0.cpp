#include "fable_ui_display_formats.h"
void __fastcall FableUiQueryDisplayExtent(void* receiver,void*,FableUiDisplayExtent* out)
{ *out=static_cast<FableUiDisplayFormatView*>(receiver)->RenderTargetDimensions; }
