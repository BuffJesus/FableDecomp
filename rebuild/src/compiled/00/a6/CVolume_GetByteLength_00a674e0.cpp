#include "fable_volume_types.h"
unsigned long CVolume::GetByteLength() const
{
    D3DVOLUME_DESC dimensions;
    PD3DVolume->GetDesc(&dimensions);
    D3DVOLUME_DESC format;
    PD3DVolume->GetDesc(&format);
    CPixelFormat pixelFormat(format.Format);
    return pixelFormat.GetByteColourDepth() * dimensions.Depth * dimensions.Height * dimensions.Width;
}
