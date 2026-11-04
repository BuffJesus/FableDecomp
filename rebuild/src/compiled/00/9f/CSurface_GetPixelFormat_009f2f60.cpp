// CSurface::GetPixelFormat (retail 0x009F2F60).
// FableWin.pdb ?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ.
#include "fable_surface.h"

CPixelFormat CSurface::GetPixelFormat() const
{
    D3DSURFACE_DESC desc;
    PD3DSurface->GetDesc(&desc);
    return CPixelFormat(desc.Format);
}