#include "fable_visual_boot.h"
#include "fable_video_system.h"

#include <stdio.h>

extern "C" __declspec(dllimport)
FableInstanceHandle FABLE_STDCALL GetModuleHandleA(const char* moduleName);

int main()
{
    const fable_u8 source[] = {
        0xFF, 0x00, 0x00,
        0xFF, 0xFF, 0xFF,
        0xCD, 0xCD,
        0x00, 0x00, 0xFF,
        0x00, 0xFF, 0x00,
        0xCD, 0xCD
    };
    fable_u8 rgba[20];
    fable_u8 rgb1555[12];
    for (unsigned int byte = 0; byte < sizeof(rgba); ++byte)
        rgba[byte] = 0xCD;
    for (unsigned int wordByte = 0;
         wordByte < sizeof(rgb1555);
         ++wordByte)
    {
        rgb1555[wordByte] = 0xCD;
    }

    if (
        !FableConvertVideoRgb24Frame(
            0x15,
            source,
            8,
            rgba,
            10,
            2,
            2) ||
        rgba[0] != 0xFF ||
        rgba[1] != 0x00 ||
        rgba[2] != 0x00 ||
        rgba[3] != 0xFF ||
        rgba[4] != 0xFF ||
        rgba[5] != 0xFF ||
        rgba[6] != 0xFF ||
        rgba[7] != 0xFF ||
        rgba[8] != 0xCD ||
        rgba[9] != 0xCD ||
        rgba[10] != 0x00 ||
        rgba[11] != 0x00 ||
        rgba[12] != 0xFF ||
        rgba[13] != 0xFF ||
        rgba[14] != 0x00 ||
        rgba[15] != 0xFF ||
        rgba[16] != 0x00 ||
        rgba[17] != 0xFF ||
        rgba[18] != 0xCD ||
        rgba[19] != 0xCD)
    {
        return 2;
    }
    if (
        !FableConvertVideoRgb24Frame(
            0x19,
            source,
            8,
            rgb1555,
            6,
            2,
            2) ||
        rgb1555[0] != 0x1F ||
        rgb1555[1] != 0x80 ||
        rgb1555[2] != 0xFF ||
        rgb1555[3] != 0xFF ||
        rgb1555[4] != 0xCD ||
        rgb1555[5] != 0xCD ||
        rgb1555[6] != 0x00 ||
        rgb1555[7] != 0xFC ||
        rgb1555[8] != 0xE0 ||
        rgb1555[9] != 0x83 ||
        rgb1555[10] != 0xCD ||
        rgb1555[11] != 0xCD ||
        FableConvertVideoRgb24Frame(
            0x18,
            source,
            8,
            rgb1555,
            6,
            2,
            2))
    {
        return 3;
    }

#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    if (!FableIsRetailVisualAssetEmbedded())
        return 1;
#else
    if (FableIsRetailVisualAssetEmbedded())
        return 1;
#endif

    char verificationCommand[] = "--verify-visual-resource";
    const long result = FableRunVisualBootCheckpoint(
        GetModuleHandleA(0),
        verificationCommand,
        0);
    if (result != 0)
        return static_cast<int>(result);

    printf("FABLETLC_VISUAL_BOOT_BEHAVIOR PASS\n");
    return 0;
}
