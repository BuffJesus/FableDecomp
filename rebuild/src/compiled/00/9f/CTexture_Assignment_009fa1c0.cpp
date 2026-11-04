#include "fable_surface_types.h"

const CTexture& CTexture::operator=(const CTexture& rhs)
{
    if (PD3DTexture != 0)
    {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }

    PD3DTexture = rhs.PD3DTexture;
    ByteLength = rhs.ByteLength;
    AllocationSource = rhs.AllocationSource;
    if (PD3DTexture != 0)
    {
        PD3DTexture->AddRef();
    }
    return *this;
}
