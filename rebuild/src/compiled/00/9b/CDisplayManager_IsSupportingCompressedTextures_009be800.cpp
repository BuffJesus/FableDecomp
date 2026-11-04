// CDisplayManager::IsSupportingCompressedTextures (retail 0x009BE800).
// FableWin.pdb ?IsSupportingCompressedTextures@CDisplayManager@@QAE_NXZ.
#include "fable_display_viewport.h"

bool CDisplayManager::IsSupportingCompressedTextures()
{
    if (D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal, D3DDeviceCaps.DeviceType,
                                  CurrentMode.Format, 0, D3DRTYPE_TEXTURE, D3DFMT_DXT1) >= 0)
        return true;
    return false;}
