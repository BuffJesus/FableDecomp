// CDisplayManager::GetBestCompressedOpaqueTextureFormat (retail 0x009BE830).
// FableWin.pdb ?GetBestCompressedOpaqueTextureFormat@CDisplayManager@@QAE_NKAAVCPixelFormat@@@Z.
// Name/address pairing from CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3.
#include "fable_surface.h"

bool CDisplayManager::GetBestCompressedOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format)
{
    if (D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                               CurrentMode.Format, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1) >= 0)
    {
        format.Initialise(D3DFMT_DXT1);
        return true;
    }
    return false;
}