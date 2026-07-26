#include "rebuild_abi.h"

struct FableDirect3DTexture;
typedef fable_u32 (__stdcall *FableTextureReleaseCall)(
    FableDirect3DTexture* texture);

struct FableDirect3DTextureVTable
{
    void* queryInterface00;
    void* addRef04;
    FableTextureReleaseCall release08;
};

struct FableDirect3DTexture
{
    FableDirect3DTextureVTable* vtable;
};

struct CTextureUninitialiseView
{
    FableDirect3DTexture* texture00;
    fable_u32 flags04;

    void Uninitialise();
};

void CTextureUninitialiseView::Uninitialise()
{
    if (texture00 != 0)
    {
        texture00->vtable->release08(texture00);
        texture00 = 0;
        flags04 &= 0x0FFFFFFF;
    }
}
