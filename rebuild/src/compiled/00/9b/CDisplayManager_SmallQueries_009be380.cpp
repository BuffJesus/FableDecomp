// lib_display_manager.cpp, retail 0x009BE330-0x009BEE3F and 0x009BF5E0 (Ego_r CDisplayManager
// names): small device and state queries.
#include "fable_display_viewport.h"
#include "fable_display_geometry.h"
#include "fable_exception.h"

// Retail 009BE340: the adapter's current refresh rate; failure throws.
long CDisplayManager::ReadDeviceRefreshRate()
{
    D3DDISPLAYMODE mode;
    if (D3D->GetAdapterDisplayMode(0, &mode) < 0)  // FAILED
        throw CGenericException();
    return mode.RefreshRate;
}

// Retail 009BE330: empty on PC.
void CDisplayManager::ChangePresentationIntervalLockToGivenFrameRateOrLower(float, bool)
{
}

// Retail 009BE380: seconds per presented frame at this refresh rate.
float CDisplayManager::CalcPresentationIntervalTime(long refresh_rate, long interval)
{
    return (float)interval / refresh_rate;
}

// Retail 009BE400.
void CDisplayManager::FlushVideoMemeory()
{
    D3DDevice->EvictManagedResources();
}

// Retail 009BE410.
long CDisplayManager::GetAvailableTextureMemory()
{
    return D3DDevice->GetAvailableTextureMem();
}

// Retail 009BECB0: the whole back buffer.
C2DBoxF CDisplayManager::GetAVisibleTelevisionScreenArea() const
{
    return C2DBoxF(0.0f, 0.0f, (float)BackBufferDimensionsStorage_[0], (float)BackBufferDimensionsStorage_[1]);
}

// Retail 009BED80: sizes shrink with the multisample factor (2x: half, 3x: a
// third). Near match, not gated: retail keeps the scale on the FPU stack like
// this double form, but loads its constants as floats (dword) where this loads
// doubles; a float `scale` is spilled to the stack instead.
float CDisplayManager::ScaleForAntialiasing(float value)
{
    double scale = 1.0f;
    switch (MultisampleType)
    {
    case 4:
        scale = 0.5f;
        break;
    case 9:
        scale = 1.0f / 3.0f;
        break;
    }
    return (float)(value * scale);
}

// Retail 009BEDE0: the caps' vertex shader version as major*1000 + minor*100.
unsigned long CDisplayManager::GetVertexShaderVersion() const
{
    return (((D3DDeviceCaps.VertexShaderVersion >> 8) & 0xFF) * 10 + (D3DDeviceCaps.VertexShaderVersion & 0xFF)) * 100;
}

// Retail 009BEE20 / 009BEE30: the tile allocator is unused on PC.
void CDisplayManager::ReleaseTile(long, CSurface&)
{
}

long CDisplayManager::AllocTile(CSurface&, bool)
{
    return 0;
}

// Retail 009BF5E0: the back buffer into its copy texture.
CTexture& CDisplayManager::CopyBackBufferToTexture()
{
    BlitRenderTargetSurfaceToTexture(BackBufferCopyTexture, BackBuffer);
    return BackBufferCopyTexture;
}
