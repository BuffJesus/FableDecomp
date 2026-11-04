#include "engine/CRenderTarget.h"
void CRenderTarget::Uninitialise()
{
    if(RenderToSurface.PD3DSurface)RenderToSurface.Uninitialise();
    if(ZBufferSurface.PD3DSurface)ZBufferSurface.Uninitialise();
}
