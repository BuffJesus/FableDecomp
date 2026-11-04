// CRenderTarget::DetachTarget / DetachZBuffer (Ego_r ?DetachTarget@CRenderTarget@@QAEXXZ and
// ?DetachZBuffer@CRenderTarget@@QAEXXZ, Ego_r 0xB7EE20/0xB7EE30 between the constructor and
// Uninitialise, exactly as retail 0xA10DC0 ctor, 0xA10E10, 0xA10E20, 0xA10E30 Uninitialise):
// release one attached surface when it holds a D3D surface (CSurface::Uninitialise 0x009F2E20).
#include "engine/CRenderTarget.h"

void CRenderTarget::DetachTarget()
{
    if (RenderToSurface.PD3DSurface)
        RenderToSurface.Uninitialise();
}

void CRenderTarget::DetachZBuffer()
{
    if (ZBufferSurface.PD3DSurface)
        ZBufferSurface.Uninitialise();
}
