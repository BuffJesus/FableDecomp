// CDisplayManager::ClearRenderTarget (retail 0x009BE420).
// FableWin.pdb ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z.
// Flags: 1 = depth, 2 = stencil (only when the depth format has stencil bits),
// 4 = colour target; clears with Z = 1.0, stencil 0.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include "fable_rgb_colour.h"

void CDisplayManager::ClearRenderTarget(const CRGBColour& colour, unsigned long flags)
{
    DWORD clearFlags = 0;
    if (flags & 1)
        clearFlags = D3DCLEAR_ZBUFFER;
    if (flags & 2)
    {
        const long index = CPixelFormat::FindFormatInfoIndexFromD3DFormat(
            GD3DFormats[GetDepthBufferFormat().FormatIndex].D3DFormat);
        if (GD3DFormats[index].StencilBits)
            clearFlags |= D3DCLEAR_STENCIL;
    }
    if (flags & 4)
        clearFlags |= D3DCLEAR_TARGET;
    D3DDevice->Clear(0, 0, clearFlags, colour.GetD3DCOLOR(), 1.0f, 0);
}
