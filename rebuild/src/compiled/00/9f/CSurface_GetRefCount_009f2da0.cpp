// CSurface::GetRefCount (retail 0x009F2DA0; Ego_r ?GetRefCount@CSurface@@QBEJXZ,
// between the copy constructor 0x009F2D60 and Unlock 0x009F2DC0 in module order).
// D3D9 has no reference-count query, so the engine bumps the count and returns
// what Release reports.
#include "fable_surface.h"

long CSurface::GetRefCount() const
{
    if (PD3DSurface)
    {
        PD3DSurface->AddRef();
        return PD3DSurface->Release();
    }
    return 0;
}
