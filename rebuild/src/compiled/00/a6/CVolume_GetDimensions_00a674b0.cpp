#include "fable_volume_types.h"
C3DExtentsI CVolume::GetDimensions() const
{
    D3DVOLUME_DESC description;
    PD3DVolume->GetDesc(&description);
    return C3DExtentsI(description.Width, description.Height, description.Depth);
}
