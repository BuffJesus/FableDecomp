// CDisplayManager::IsTextureBitDepthSupported (retail 0x009BE530).
// FableWin.pdb ?IsTextureBitDepthSupported@CDisplayManager@@QAE_NK@Z.
#include "fable_display_viewport.h"
#include "fable_d3d_format_info.h"

bool CDisplayManager::IsTextureBitDepthSupported(unsigned long bitDepth)
{
    for (const CD3DFormatInfo* info = GD3DFormats; info->BitDepth != -1; ++info)
    {
        if (info->PFType == PF_TYPE_RGBA && (long)bitDepth == info->BitDepth &&
            D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                                   CurrentMode.Format, 0, D3DRTYPE_TEXTURE,
                                   info->D3DFormat) >= 0)
            return true;
    }
    return false;
}
