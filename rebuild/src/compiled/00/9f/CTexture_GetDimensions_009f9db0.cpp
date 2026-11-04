#include "fable_surface_types.h"
C2DExtentsI CTexture::GetDimensions(unsigned long level) const
{
    D3DSURFACE_DESC description;
    PD3DTexture->GetLevelDesc(level,&description);
    return C2DExtentsI(description.Width,description.Height);
}
