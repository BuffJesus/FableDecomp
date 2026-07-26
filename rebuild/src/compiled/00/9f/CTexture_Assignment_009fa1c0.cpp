#include "rebuild_abi.h"

struct FableAssignmentTexture;

typedef fable_u32 (__stdcall *FableAssignmentTextureCall)(
    FableAssignmentTexture* texture);

struct FableAssignmentTextureVTable
{
    void* queryInterface00;
    FableAssignmentTextureCall addRef04;
    FableAssignmentTextureCall release08;
};

struct FableAssignmentTexture
{
    FableAssignmentTextureVTable* vtable;
};

struct CTextureAssignmentView
{
    FableAssignmentTexture* PD3DTexture;
    fable_u32 ByteLength : 28;
    fable_u32 AllocationSource : 4;

    CTextureAssignmentView& operator=(CTextureAssignmentView& rhs);
};

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
