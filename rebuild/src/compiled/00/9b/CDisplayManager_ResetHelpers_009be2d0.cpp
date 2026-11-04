#include "fable_surface.h"

unsigned int CDisplayManager::CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc)
{
    if (!desc.PresentImmediate)
    {
        switch (desc.VSyncCount)
        {
        case 0: case 1: return 1;
        case 2: return 2;
        case 3: return 4;
        case 4: return 8;
        }
    }
    return 0x80000000;
}

bool CDisplayManager::SetupDepthAndBackBufferTextures()
{
    CPixelFormat format = RenderTargetSurface.GetPixelFormat();
    C2DExtentsI dimensions;
    const bool powerOfTwo = ((D3DDeviceCaps.TextureCaps >> 1) & 1) != 0;
    if (powerOfTwo)
        dimensions.X = dimensions.Y = 1024;
    else
    {
        dimensions.X = RenderTargetDimensionsStorage_[0];
        dimensions.Y = RenderTargetDimensionsStorage_[1];
    }
    if (!BackBufferCopyTexture.Initialise(
        dimensions,1,format,1,SURFACE_POOL_DEFAULT_LOCATION,false,false))
        return false;
    return true;
}
