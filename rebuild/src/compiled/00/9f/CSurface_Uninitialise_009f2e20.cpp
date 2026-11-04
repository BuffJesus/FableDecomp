#include "fable_surface_types.h"
void CSurface::Uninitialise()
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
}
