#include "fable_ui_texture_surfaces.h"
void __fastcall FableUiAttachSurface(CSurface* surface,void*,IDirect3DSurface9* native)
{
    FableUiReleaseSurface(surface,0);
    surface->PD3DSurface=native;
    surface->AllocationSource=3;
}
