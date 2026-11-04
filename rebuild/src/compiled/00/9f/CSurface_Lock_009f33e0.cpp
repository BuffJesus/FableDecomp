// lib_surface.cpp, retail 0x009F33E0 (Ego_r
// ?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z): lock the whole surface.
// The result carries the surface size, pitch and bits, or zeros if the lock fails.
// Near match, not gated: retail keeps width/height in ebx/ebp across the calls
// (four callee-saved registers, a 0x38-byte frame); none of the forms tried
// reproduce that allocation.
#include "fable_surface_types.h"

CSurfaceLockedRect CSurface::Lock(unsigned long flags)
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    RECT rect = {0, 0, desc.Width, desc.Height};
    D3DLOCKED_RECT locked;
    CSurfaceLockedRect result;
    if (PD3DSurface->LockRect(&locked, &rect, flags) >= 0)
    {
        result.Dimensions = C2DExtentsI(desc.Width, desc.Height);
        result.Pitch = locked.Pitch;
        result.PMem = (unsigned char*)locked.pBits;
    }
    else
    {
        result.Dimensions = C2DExtentsI(0, 0);
        result.Pitch = 0;
        result.PMem = 0;
    }
    return result;
}
