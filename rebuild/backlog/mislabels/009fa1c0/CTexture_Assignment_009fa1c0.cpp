#include "fable_texture_lifecycle.h"

CTextureAssignmentView& CTextureAssignmentView::operator=(
    CTextureAssignmentView& rhs)
{
    if (PD3DTexture != 0)
    {
        PD3DTexture->vtable->release08(PD3DTexture);
        PD3DTexture = 0;
        AllocationSource = 0;
    }

    PD3DTexture = rhs.PD3DTexture;
    ByteLength = rhs.ByteLength;
    AllocationSource = rhs.AllocationSource;
    if (PD3DTexture != 0)
    {
        PD3DTexture->vtable->addRef04(PD3DTexture);
    }
    return *this;
}
