#include "fable_volume_types.h"
unsigned int CVolumeTexture::GetLevelByteLength(unsigned int level) const
{
    D3DVOLUME_DESC dimensions;
    PD3DVolumeTexture->GetLevelDesc(level, &dimensions);
    D3DVOLUME_DESC format;
    PD3DVolumeTexture->GetLevelDesc(0, &format);
    CPixelFormat pixelFormat(format.Format);
    return pixelFormat.GetByteColourDepth() * dimensions.Depth * dimensions.Height * dimensions.Width;
}
