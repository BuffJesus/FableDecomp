#include "fable_surface_types.h"

void CTexture::Uninitialise()
{
    if (PD3DTexture != 0)
    {
        PD3DTexture->Release();
        PD3DTexture = 0;
        AllocationSource = 0;
    }
}
