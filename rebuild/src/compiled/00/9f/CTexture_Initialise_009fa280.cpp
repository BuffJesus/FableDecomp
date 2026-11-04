#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"
bool CTexture::Initialise(const C2DExtentsI& size,long levels,const CPixelFormat& format,unsigned long flags,ESurfacePool pool,bool dynamic,bool automaticMipmaps)
{
    if(PD3DTexture) {
        PD3DTexture->Release();
        PD3DTexture=0;
        AllocationSource=0;
    }
    if(levels<=0) {
        unsigned long width=size.X,height=size.Y,count=0;
        while(width>=8 && height>=8) {
            width>>=1;
            height>>=1;
            ++count;
        }
        levels=count+1;
    }
    IDirect3DDevice9* device=GFGetSystemManager()->GetDisplayManager()->D3DDevice;
    D3DFORMAT d3dFormat=GD3DFormats[format.FormatIndex].D3DFormat;
    HRESULT result=device->CreateTexture(size.X,size.Y,levels,flags,d3dFormat,
        (D3DPOOL)pool,&PD3DTexture,0);
    if(result<0)return false;
    AllocationSource=1;
    CalcByteLength();
    return true;
}
