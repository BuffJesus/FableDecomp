// lib_display_manager.cpp, retail 0x009BF160 / 0x009BF220 (Ego_r
// ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z and
// ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ): adopt a target (or
// the back buffer) as the current render surfaces, re-derive the virtual
// pixel mapping, bind both surfaces and reset the viewport to the whole
// target. Supersedes the stand-in "View" class landings at the same addresses.
#include "fable_display_viewport.h"
#include "engine/CRenderTarget.h"
#include "fable_display_geometry.h"

// Retail 009BF160.
bool CDisplayManager::SetRenderTarget(CRenderTarget& target)
{
    RenderTargetSurface = target.RenderToSurface;
    CSurface* depth = &target.ZBufferSurface;
    RenderTargetDepthBuffer = *depth;
    RenderTargetDimensions() = RenderTargetSurface.GetDimensions();
    InitVirtualPixels();
    IDirect3DSurface9* depth_surface = 0;
    if (depth != 0)
        depth_surface = target.ZBufferSurface.GetD3DSurface();
    if (D3DDevice->SetRenderTarget(0, RenderTargetSurface.GetD3DSurface()) >= 0 &&
        D3DDevice->SetDepthStencilSurface(depth_surface) >= 0)
    {
        C2DBoxI viewport;
        viewport.left = 0;
        viewport.top = 0;
        viewport.right = RenderTargetDimensions().X;
        viewport.bottom = RenderTargetDimensions().Y;
        SetViewport(viewport);
        return true;
    }
    return false;
}

// Retail 009BF220.
bool CDisplayManager::SetRenderTargetAsBackScreen()
{
    RenderTargetSurface = BackBuffer;
    RenderTargetDepthBuffer = DepthBuffer;
    RenderTargetDimensions() = RenderTargetSurface.GetDimensions();
    InitVirtualPixels();
    if (D3DDevice->SetRenderTarget(0, BackBuffer.GetD3DSurface()) >= 0 &&
        D3DDevice->SetDepthStencilSurface(DepthBuffer.GetD3DSurface()) >= 0)
    {
        C2DBoxI viewport;
        viewport.left = 0;
        viewport.top = 0;
        viewport.right = RenderTargetDimensions().X;
        viewport.bottom = RenderTargetDimensions().Y;
        SetViewport(viewport);
        return true;
    }
    return false;
}
