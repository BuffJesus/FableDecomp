#include "fable_display_viewport.h"
#include "fable_d3d_format_info.h"

// Retail 009BE3C0: the adapter accepts this multisample type for the format
// as a full-screen surface.
bool CDisplayManager::IsAntiAliasingModeValid(ESurfaceMultisampleType type, const CPixelFormat& format)
{
    D3DFORMAT surfaceFormat = GD3DFormats[format.FormatIndex].D3DFormat;
    return D3D->CheckDeviceMultiSampleType(0, D3DDeviceCaps.DeviceType, surfaceFormat, 0, (D3DMULTISAMPLE_TYPE)type, 0) == 0;
}

bool CDisplayManager::IsValidTextureFormat(const CPixelFormat& format)
{
    D3DFORMAT textureFormat=GD3DFormats[format.FormatIndex].D3DFormat;
    return D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,
        CurrentMode.Format,0,D3DRTYPE_TEXTURE,textureFormat)>=0;
}
bool CDisplayManager::IsValidRenderTargetFormat(const CPixelFormat& colour,const CPixelFormat& depth)
{
    D3DFORMAT depthFormat=GD3DFormats[depth.FormatIndex].D3DFormat;
    D3DFORMAT colourFormat=GD3DFormats[colour.FormatIndex].D3DFormat;
    return D3D->CheckDepthStencilMatch(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,
        CurrentMode.Format,colourFormat,depthFormat)>=0;
}
