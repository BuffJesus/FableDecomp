#include "fable_surface_types.h"
CSurface CTexture::GetSurfaceLevel(long level) const
{
    IDirect3DSurface9* d3dSurface;
    PD3DTexture->GetSurfaceLevel(level, &d3dSurface);

    CSurface surface;
    surface.InitialiseFromTextureMipmap(d3dSurface);
    return surface;
}


