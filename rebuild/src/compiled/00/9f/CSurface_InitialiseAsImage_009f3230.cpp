#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"

bool CSurface::InitialiseAsImage(const C2DExtentsI& size,const CPixelFormat& format,bool dynamic)
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface=0;
        AllocationSource=0;
    }
    const long width=size.X,height=size.Y;
    IDirect3DDevice9* device=GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    D3DFORMAT d3dFormat=GD3DFormats[format.FormatIndex].D3DFormat;
    bool result=device->CreateOffscreenPlainSurface(width,height,d3dFormat,D3DPOOL_SCRATCH,&PD3DSurface,0)>=0;
    AllocationSource=1;
    return result;
}
