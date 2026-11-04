#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"
bool CSurface::InitialiseAsDepthBuffer(const C2DExtentsI& size,const CPixelFormat& format,ESurfaceMultisampleType multisample,bool discard)
{
    if(PD3DSurface) {
        PD3DSurface->Release();
        PD3DSurface=0;
        AllocationSource=0;
    }
    IDirect3DDevice9* device=GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    D3DFORMAT d3dFormat=GD3DFormats[format.FormatIndex].D3DFormat;
    bool result=device->CreateDepthStencilSurface(size.X,size.Y,d3dFormat,
        (D3DMULTISAMPLE_TYPE)multisample,0,false,&PD3DSurface,0)>=0;
    AllocationSource=1;
    return result;
}
