#include "fable_surface_types.h"
CSurface& CSurface::operator=(const CSurface& source)
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
    PD3DSurface = source.PD3DSurface;
    AllocationSource = source.AllocationSource;
    if(AllocationSource == 2)PAllocatedMemory = source.PAllocatedMemory;
    if(PD3DSurface)PD3DSurface->AddRef();
    return *this;
}
