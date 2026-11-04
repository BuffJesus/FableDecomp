// lib_surface.cpp, retail 0x009F3310 (Ego_r
// ?InitialiseAsRenderTarget@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@_N3@Z):
// drop any previous surface, then create a render target on the display
// device. The last flag is unused in retail.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"

bool CSurface::InitialiseAsRenderTarget(const C2DExtentsI& size, const CPixelFormat& format,
                                        ESurfaceMultisampleType multisample, bool lockable, bool unused)
{
    if (PD3DSurface)
    {
        PD3DSurface->Release();
        PD3DSurface = 0;
        AllocationSource = 0;
    }
    long width = size.X;
    long height = size.Y;
    IDirect3DDevice9* device = GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    D3DFORMAT d3dFormat = GD3DFormats[format.FormatIndex].D3DFormat;
    bool result = device->CreateRenderTarget(width, height, d3dFormat, (D3DMULTISAMPLE_TYPE)multisample, 0, lockable,
                                             &PD3DSurface, 0) >= 0;
    AllocationSource = 1;
    return result;
}
