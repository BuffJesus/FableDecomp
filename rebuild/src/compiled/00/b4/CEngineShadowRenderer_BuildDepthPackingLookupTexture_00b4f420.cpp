#include "engine/CEngineShadowRenderer.h"
#include "fable_rgb_colour.h"
void CEngineShadowRenderer::BuildDepthPackingLookupTexture()
{
    CPixelFormat format;
    format.Initialise(8,8,8,8,false);
    DepthPackingLookupTexture.Initialise(C2DExtentsI(256,256),1,format,0,SURFACE_POOL_MANAGED,false,false);
    CSurface surface=DepthPackingLookupTexture.GetSurfaceLevel(0);
    CSurfaceLockedRect rect=surface.Lock(0);
    for(long y=0;y<256;++y)
        for(long x=0;x<256;++x)
            surface.SetPixel(x,y,CRGBColour((unsigned char)x,(unsigned char)(y&0xfe),0,0),rect);
    surface.Unlock();
}
