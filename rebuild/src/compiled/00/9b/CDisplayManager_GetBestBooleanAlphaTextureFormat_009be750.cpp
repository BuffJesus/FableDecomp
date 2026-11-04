// CDisplayManager::GetBestBooleanAlphaTextureFormat (retail 0x009BE750).
// FableWin.pdb ?GetBestBooleanAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z.
// RGBA entry of the requested depth with the fewest (but some) alpha bits.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

bool CDisplayManager::GetBestBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i)
    {
        const CD3DFormatInfo* info = &GD3DFormats[i];
        if (info->PFType == PF_TYPE_RGBA && info->BitDepth == (long)bitDepth &&
            info->RBits > 0 && info->GBits > 0 && info->BBits > 0 && info->AlphaBits > 0 &&
            (!best || info->AlphaBits < best->AlphaBits) &&
            D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                                   CurrentMode.Format, 0, D3DRTYPE_TEXTURE, info->D3DFormat) >= 0)
            best = info;
    }
    if (best)
    {
        format.Initialise(best->D3DFormat);
        return true;
    }
    return false;
}