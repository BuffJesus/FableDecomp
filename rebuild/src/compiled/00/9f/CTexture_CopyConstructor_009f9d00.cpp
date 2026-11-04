#include "fable_surface_types.h"
CTexture::CTexture(const CTexture& source)
    : PD3DTexture(source.PD3DTexture), ByteLength(source.ByteLength),
      AllocationSource(source.AllocationSource)
{
    if (PD3DTexture)
        PD3DTexture->AddRef();
}
