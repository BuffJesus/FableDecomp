#include "fable_volume_types.h"
CPixelFormat CVolumeTexture::GetPixelFormat() const
{
    D3DVOLUME_DESC description;
    PD3DVolumeTexture->GetLevelDesc(0, &description);
    return CPixelFormat(description.Format);
}
