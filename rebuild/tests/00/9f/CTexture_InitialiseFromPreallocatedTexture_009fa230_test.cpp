#include <stdio.h>

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

static int g_ReleaseCalls = 0;
static int g_CalcCalls = 0;

static fable_u32 __stdcall ReleaseTexture(
    FablePreallocatedTexture*)
{
    ++g_ReleaseCalls;
    return 0;
}

void CTexturePreallocatedView::CalcByteLength()
{
    ++g_CalcCalls;
}

int main()
{
    FablePreallocatedTextureVTable vtable =
        {0, 0, &ReleaseTexture};
    FablePreallocatedTexture oldTexture = {&vtable};
    FablePreallocatedTexture newTexture = {&vtable};
    CTexturePreallocatedView owner =
        {&oldTexture, 0xF1234567};

    if (!owner.InitialiseFromPreallocatedTexture(&newTexture))
    {
        return 1;
    }
    if (
        owner.texture00 != &newTexture ||
        owner.flags04 != 0x11234567 ||
        g_ReleaseCalls != 1 ||
        g_CalcCalls != 1)
    {
        return 1;
    }

    if (!owner.InitialiseFromPreallocatedTexture(0))
    {
        return 1;
    }
    if (
        owner.texture00 != 0 ||
        owner.flags04 != 0x01234567 ||
        g_ReleaseCalls != 2 ||
        g_CalcCalls != 2)
    {
        return 1;
    }

    printf("FABLETLC_PREALLOCATED_TEXTURE_BEHAVIOR PASS\n");
    return 0;
}
