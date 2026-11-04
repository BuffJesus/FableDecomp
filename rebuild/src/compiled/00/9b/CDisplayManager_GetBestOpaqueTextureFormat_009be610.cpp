// CDisplayManager::GetBestOpaqueTextureFormat (retail 0x009BE610).
// FableWin.pdb ?GetBestOpaqueTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@_N@Z.
// RGB entry of the requested depth with the fewest alpha bits that the adapter
// accepts (as a render target when asked: usage 1 = D3DUSAGE_RENDERTARGET).
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

bool CDisplayManager::GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format, bool renderTarget)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i)
    {
        const CD3DFormatInfo* info = &GD3DFormats[i];
        if (info->PFType == PF_TYPE_RGBA && info->BitDepth == (long)bitDepth &&
            info->RBits > 0 && info->GBits > 0 && info->BBits > 0 &&
            (!best || info->AlphaBits < best->AlphaBits) &&
            D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                                   CurrentMode.Format, renderTarget != false, D3DRTYPE_TEXTURE,
                                   info->D3DFormat) >= 0)
            best = info;
    }
    if (best)
    {
        format.Initialise(best->D3DFormat);
        return true;
    }
    return false;
}