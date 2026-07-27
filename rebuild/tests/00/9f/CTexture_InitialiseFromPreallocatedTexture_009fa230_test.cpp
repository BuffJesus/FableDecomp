#include <stdio.h>

#include "../../../src/compiled/00/9f/CTexture_InitialiseFromPreallocatedTexture_009fa230.cpp"

static int g_ReleaseCalls = 0;
static int g_CalcCalls = 0;

static fable_u32 __stdcall ReleaseTexture(
    FableLifecycleTexture*)
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
    FableLifecycleTextureVTable vtable =
        {0, 0, &ReleaseTexture};
    FableLifecycleTexture oldTexture = {&vtable};
    FableLifecycleTexture newTexture = {&vtable};
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
