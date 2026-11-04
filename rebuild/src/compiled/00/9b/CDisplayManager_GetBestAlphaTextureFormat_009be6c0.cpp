// CDisplayManager::GetBestAlphaTextureFormat (retail 0x009BE6C0).
// FableWin.pdb ?GetBestAlphaTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z.
// First RGBA entry of the requested depth with more than one bit per channel
// (alpha included) that the adapter accepts.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

bool CDisplayManager::GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format)
{
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i)
    {
        const CD3DFormatInfo* info = &GD3DFormats[i];
        if (info->PFType == PF_TYPE_RGBA && info->BitDepth == (long)bitDepth &&
            info->RBits > 1 && info->GBits > 1 && info->BBits > 1 && info->AlphaBits > 1 &&
            D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                                   CurrentMode.Format, 0, D3DRTYPE_TEXTURE, info->D3DFormat) >= 0)
        {
            format.Initialise(info->D3DFormat);
            return true;
        }
    }
    return false;
}