#include "fable_surface_types.h"
void CSurface::InitialiseFromTextureMipmap(IDirect3DSurface9* surface)
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
    PD3DSurface = surface;
    AllocationSource = 3;
}
