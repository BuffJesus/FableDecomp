#include "fable_surface_types.h"
CSurface::CSurface(const CSurface& source)
    : PD3DSurface(source.PD3DSurface), AllocationSource(source.AllocationSource)
{
    if(AllocationSource == 2)PAllocatedMemory = source.PAllocatedMemory;
    if(PD3DSurface)PD3DSurface->AddRef();
}
