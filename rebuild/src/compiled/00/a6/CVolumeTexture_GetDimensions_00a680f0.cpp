#include "fable_volume_types.h"
C3DExtentsI CVolumeTexture::GetDimensions(unsigned long level) const
{
    D3DVOLUME_DESC description;
    PD3DVolumeTexture->GetLevelDesc(level, &description);
    return C3DExtentsI(description.Width, description.Height, description.Depth);
}
