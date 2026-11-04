// lib_render_target.cpp, retail 0x00A10DC0 / 0x00A10EC0 (Ego_r
// CRenderTarget::CRenderTarget(CSurface&, CSurface&) and
// SetZBufferAsScreenZBuffer): a target/depth surface pair; the screen form
// re-points the depth surface at the device's current depth-stencil.
#include "engine/CRenderTarget.h"
#include "fable_system_manager.h"
#include "fable_system_manager_state.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"

// Retail 00A10DC0.
CRenderTarget::CRenderTarget(CSurface& target, CSurface& zbuffer)
{
    RenderToSurface = target;
    ZBufferSurface = zbuffer;
}

// Retail 00A10EC0: GetDepthStencilSurface's reference is adopted, not added to.
void CRenderTarget::SetZBufferAsScreenZBuffer()
{
    if (ZBufferSurface.PD3DSurface)
        ZBufferSurface.Uninitialise();
    IDirect3DSurface9* surface;
    GFGetSystemManager()->PDisplayManager.Data->D3DDevice->GetDepthStencilSurface(&surface);
    ZBufferSurface.InitialiseFromPreallocatedSurface(surface, false);
}
