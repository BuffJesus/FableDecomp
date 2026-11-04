#include "engine/CRenderTarget.h"
void CRenderTarget::AttachTarget(const CSurface& surface)
{
    if(RenderToSurface.PD3DSurface)RenderToSurface.Uninitialise();
    RenderToSurface = surface;
}
void CRenderTarget::AttachZBuffer(const CSurface& surface)
{
    if(ZBufferSurface.PD3DSurface)ZBufferSurface.Uninitialise();
    ZBufferSurface = surface;
}
