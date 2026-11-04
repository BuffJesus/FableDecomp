#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"

void CSurface::InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface,bool addReference)
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface=0;
        AllocationSource=0;
    }
    PD3DSurface=surface;
    AllocationSource=surface!=0;
    if(addReference)surface->AddRef();
}

