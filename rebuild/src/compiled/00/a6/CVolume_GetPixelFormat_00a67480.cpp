#include "fable_volume_types.h"
CPixelFormat CVolume::GetPixelFormat() const
{
    D3DVOLUME_DESC description;
    PD3DVolume->GetDesc(&description);
    return CPixelFormat(description.Format);
}
