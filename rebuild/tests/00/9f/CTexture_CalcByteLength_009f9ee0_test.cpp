#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/9f/CTexture_CalcByteLength_009f9ee0.cpp"
#include "../../../src/compiled/00/9f/CTexture_InitialiseFromPreallocatedTexture_009fa230.cpp"

namespace
{
    typedef fable_u32 (__stdcall *TestGetLevelCountCall)(
        FableLifecycleTexture* texture);
    typedef fable_i32 (__stdcall *TestGetLevelDescriptionCall)(
        FableLifecycleTexture* texture,
        fable_u32 level,
        FableTextureSurfaceDescription* description);

    struct FableByteLengthTextureVTable
    {
        void* methods00[13];
        TestGetLevelCountCall getLevelCount34;
        void* methods38[3];
        TestGetLevelDescriptionCall getLevelDescription44;
    };

    struct TestTexture
    {
        FableLifecycleTextureVTable* vtable;
        fable_u32 levelCount;
        FableTextureSurfaceDescription levels[4];
    };

    fable_u32 g_LevelCountCalls;
    fable_u32 g_DescriptionCalls;
    fable_u32 g_DescriptionLevels[16];
    fable_u32 g_InitialiseCalls;
    fable_u32 g_InitialiseFormats[8];
    fable_u32 g_DepthCalls;

    void ResetCalls()
    {
        g_LevelCountCalls = 0;
        g_DescriptionCalls = 0;
        memset(g_DescriptionLevels, 0, sizeof(g_DescriptionLevels));
        g_InitialiseCalls = 0;
        memset(g_InitialiseFormats, 0, sizeof(g_InitialiseFormats));
        g_DepthCalls = 0;
    }

    fable_u32 __stdcall GetLevelCount(
        FableLifecycleTexture* texture)
    {
        ++g_LevelCountCalls;
        return reinterpret_cast<TestTexture*>(texture)->levelCount;
    }

    fable_i32 __stdcall GetLevelDescription(
        FableLifecycleTexture* texture,
        fable_u32 level,
        FableTextureSurfaceDescription* description)
    {
        g_DescriptionLevels[g_DescriptionCalls++] = level;
        *description =
            reinterpret_cast<TestTexture*>(texture)->levels[level];
        return 0;
    }

    FableByteLengthTextureVTable MakeVTable()
    {
        FableByteLengthTextureVTable vtable;
        memset(&vtable, 0, sizeof(vtable));
        vtable.getLevelCount34 = &GetLevelCount;
        vtable.getLevelDescription44 = &GetLevelDescription;
        return vtable;
    }

    FableTextureSurfaceDescription MakeDescription(
        fable_u32 format,
        fable_u32 width,
        fable_u32 height)
    {
        FableTextureSurfaceDescription description;
        memset(&description, 0, sizeof(description));
        description.format00 = format;
        description.width18 = width;
        description.height1C = height;
        return description;
    }
}

void CPixelFormatByteLengthView::Initialise(fable_u32 format)
{
    g_InitialiseFormats[g_InitialiseCalls++] = format;
    value00 = format;
}

fable_u32 CPixelFormatByteLengthView::GetColourDepth() const
{
    ++g_DepthCalls;
    if (value00 == 21)
    {
        return 32;
    }
    if (value00 == 22)
    {
        return 16;
    }
    return 8;
}

int main()
{
    CTexturePreallocatedView owner;
    owner.texture00 = 0;
    owner.flags04 = 0xF1234567;
    ResetCalls();
    owner.CalcByteLength();
    if (
        owner.flags04 != 0xF0000000 ||
        g_LevelCountCalls != 0 ||
        g_DescriptionCalls != 0 ||
        g_InitialiseCalls != 0 ||
        g_DepthCalls != 0)
    {
        printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=1\n");
        return 1;
    }

    FableByteLengthTextureVTable vtable = MakeVTable();
    TestTexture texture;
    memset(&texture, 0, sizeof(texture));
    texture.vtable =
        reinterpret_cast<FableLifecycleTextureVTable*>(&vtable);
    texture.levelCount = 3;
    texture.levels[0] = MakeDescription(21, 8, 4);
    texture.levels[1] = MakeDescription(22, 4, 2);
    texture.levels[2] = MakeDescription(99, 2, 1);

    owner.texture00 =
        reinterpret_cast<FableLifecycleTexture*>(&texture);
    owner.flags04 = 0xAFFFFFFF;
    ResetCalls();
    owner.CalcByteLength();
    if (
        owner.flags04 != 0xA00000A8 ||
        g_LevelCountCalls != 1 ||
        g_DescriptionCalls != 6 ||
        g_InitialiseCalls != 3 ||
        g_DepthCalls != 3)
    {
        printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=2\n");
        return 2;
    }
    const fable_u32 expectedLevels[6] =
        {0, 0, 1, 0, 2, 0};
    for (fable_u32 index = 0; index < 6; ++index)
    {
        if (
            g_DescriptionLevels[index] != expectedLevels[index] ||
            g_InitialiseFormats[index / 2] != 21)
        {
            printf(
                "FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=3\n");
            return 3;
        }
    }

    texture.levelCount = 0;
    owner.flags04 = 0x71234567;
    ResetCalls();
    owner.CalcByteLength();
    if (
        owner.flags04 != 0x70000000 ||
        g_LevelCountCalls != 1 ||
        g_DescriptionCalls != 0 ||
        g_InitialiseCalls != 0)
    {
        printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=4\n");
        return 4;
    }

    texture.levelCount = 1;
    texture.levels[0] =
        MakeDescription(21, 0x04000001, 1);
    owner.flags04 = 0xB7654321;
    ResetCalls();
    owner.CalcByteLength();
    if (
        owner.flags04 != 0xB0000004 ||
        g_DescriptionCalls != 2 ||
        g_InitialiseFormats[0] != 21)
    {
        printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=5\n");
        return 5;
    }

    texture.levelCount = 3;
    texture.levels[0] = MakeDescription(21, 8, 4);
    texture.levels[1] = MakeDescription(22, 4, 2);
    texture.levels[2] = MakeDescription(99, 2, 1);
    owner.texture00 = 0;
    owner.flags04 = 0xF1234567;
    ResetCalls();
    if (
        !owner.InitialiseFromPreallocatedTexture(
            reinterpret_cast<FableLifecycleTexture*>(&texture)) ||
        owner.texture00 !=
            reinterpret_cast<FableLifecycleTexture*>(&texture) ||
        owner.flags04 != 0x100000A8 ||
        g_LevelCountCalls != 1 ||
        g_DescriptionCalls != 6)
    {
        printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH FAIL code=6\n");
        return 6;
    }

    printf("FABLETLC_TEXTURE_CALC_BYTE_LENGTH PASS\n");
    return 0;
}
