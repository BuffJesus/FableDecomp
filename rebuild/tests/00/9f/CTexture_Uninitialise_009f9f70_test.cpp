#include <stdio.h>

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

static int g_ReleaseCalls = 0;
static FableDirect3DTexture* g_ReleasedTexture = 0;

static fable_u32 __stdcall ReleaseTexture(
    FableDirect3DTexture* texture)
{
    ++g_ReleaseCalls;
    g_ReleasedTexture = texture;
    return 0;
}

int main()
{
    FableDirect3DTextureVTable vtable = {0, 0, &ReleaseTexture};
    FableDirect3DTexture texture = {&vtable};
    CTextureUninitialiseView owner = {&texture, 0xF1234567};

    owner.Uninitialise();
    if (
        owner.texture00 != 0 ||
        owner.flags04 != 0x01234567 ||
        g_ReleaseCalls != 1 ||
        g_ReleasedTexture != &texture)
    {
        return 1;
    }

    owner.Uninitialise();
    if (g_ReleaseCalls != 1)
    {
        return 1;
    }

    printf("FABLETLC_TEXTURE_UNINITIALISE_BEHAVIOR PASS\n");
    return 0;
}
