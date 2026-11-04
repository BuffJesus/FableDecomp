#include "fable_display_viewport.h"
#include "engine/CRenderTarget.h"

// Ego_r ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z, retail 0x009BF160.
bool CDisplayManager::SetRenderTarget(CRenderTarget& target)
{
    CSurface* renderColour = &RenderTargetSurface;
    *renderColour = target.RenderToSurface;
    CSurface* targetDepth = &target.ZBufferSurface;
    RenderTargetDepthBuffer = *targetDepth;
    RenderTargetDimensions() = renderColour->GetDimensions();
    InitVirtualPixels();

    IDirect3DSurface9* depthSurface = 0;
    if (targetDepth != 0)
        depthSurface = target.ZBufferSurface.GetD3DSurface();

    if (D3DDevice->SetRenderTarget(
            0,
            RenderTargetSurface.GetD3DSurface()) >= 0)
    {
        if (D3DDevice->SetDepthStencilSurface(
                depthSurface) >= 0)
        {
            C2DBoxI viewport = {
                0,
                0,
                RenderTargetDimensions().X,
                RenderTargetDimensions().Y
            };
            SetViewport(viewport);
            return true;
        }
    }
    return false;
}
