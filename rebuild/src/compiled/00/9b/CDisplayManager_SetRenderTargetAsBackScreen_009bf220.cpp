// lib_display_manager.cpp, retail 0x009BF220 (Ego_r
// ?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ): adopt the back
// buffer and its depth buffer as the current render surfaces, re-derive the
// virtual pixel mapping, bind both surfaces and reset the viewport to the
// whole target.
#include "fable_display_viewport.h"
#include "fable_display_geometry.h"

bool CDisplayManager::SetRenderTargetAsBackScreen()
{
    RenderTargetSurface = BackBuffer;
    RenderTargetDepthBuffer = DepthBuffer;
    RenderTargetDimensions() =
        RenderTargetSurface.GetDimensions();
    InitVirtualPixels();

    if (D3DDevice->SetRenderTarget(
            0,
            BackBuffer.GetD3DSurface()) >= 0)
    {
        if (D3DDevice->SetDepthStencilSurface(
                DepthBuffer.GetD3DSurface()) >= 0)
        {
            C2DBoxI viewport;
            viewport.left = 0;
            viewport.top = 0;
            viewport.right = RenderTargetDimensions().X;
            viewport.bottom = RenderTargetDimensions().Y;
            SetViewport(viewport);
            return true;
        }
    }
    return false;
}
