#pragma once
#include "fable_surface_types.h"
#include "fable_display_viewport.h"

// ?GetBackBuffer@CDisplayManager@@QAEAAVCSurface@@XZ; inline in retail.
inline CSurface& CDisplayManager::GetBackBuffer()
{
    return BackBuffer;
}

// ?GetDepthBufferFormat@CDisplayManager@@QAEABVCPixelFormat@@XZ (FableWin);
// retail CDisplayManager::ClearRenderTarget 0x009BE420 reads +0x1AC directly.
inline const CPixelFormat& CDisplayManager::GetDepthBufferFormat()
{
    return *reinterpret_cast<const CPixelFormat*>(&DepthBufferFormatStorage_);
}

typedef char FableCSurfaceSizeMustBe16[sizeof(CSurface) == 0x10 ? 1 : -1];
typedef char FableCTextureSizeMustBe8[sizeof(CTexture) == 0x8 ? 1 : -1];

