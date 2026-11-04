// CDisplayManager::GetBestSignedTextureFormat (retail 0x009BE590).
// FableWin.pdb ?GetBestSignedTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z.
// First PF_TYPE_BUMP entry of the requested bit depth the adapter accepts.
#include "fable_surface.h"
#include "fable_d3d_format_info.h"

bool CDisplayManager::GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& format)
{
    const CD3DFormatInfo* best = 0;
    for (long i = 0; GD3DFormats[i].BitDepth != -1; ++i)
    {
        const CD3DFormatInfo* info = &GD3DFormats[i];
        if (info->PFType == PF_TYPE_BUMP && info->BitDepth == (long)bitDepth && !best &&
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