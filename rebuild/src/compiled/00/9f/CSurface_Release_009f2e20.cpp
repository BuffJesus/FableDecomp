#include "fable_ui_texture_surfaces.h"
void __fastcall FableUiReleaseSurface(CSurface* surface,void*)
{
    FableUiNativeSurface* native=FableUiNative(surface);
    if(native)
    {
        native->Vtable->Release(native);
        surface->PD3DSurface=0;
        surface->AllocationSource=0;
    }
}
