#include "rebuild_abi.h"

struct FablePreallocatedTexture;
typedef fable_u32 (__stdcall *FablePreallocatedTextureReleaseCall)(
    FablePreallocatedTexture* texture);

struct FablePreallocatedTextureVTable
{
    void* queryInterface00;
    void* addRef04;
    FablePreallocatedTextureReleaseCall release08;
};

struct FablePreallocatedTexture
{
    FablePreallocatedTextureVTable* vtable;
};

struct CTexturePreallocatedView
{
    FablePreallocatedTexture* texture00;
    fable_u32 flags04;

    void CalcByteLength();
    bool InitialiseFromPreallocatedTexture(
        FablePreallocatedTexture* texture);
};

bool CTexturePreallocatedView::InitialiseFromPreallocatedTexture(
    FablePreallocatedTexture* texture)
{
    if (texture00 != 0)
    {
        texture00->vtable->release08(texture00);
        texture00 = 0;
        flags04 &= 0x0FFFFFFF;
    }

    texture00 = texture;
    const fable_u32 hasTexture =
        static_cast<fable_u32>(texture != 0);
    flags04 =
        (flags04 & 0x0FFFFFFF) |
        (hasTexture << 28);
    CalcByteLength();
    return true;
}
