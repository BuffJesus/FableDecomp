#include "fable_video_system.h"

bool FABLE_FASTCALL FableConvertVideoRgb24Frame(
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u8* destination,
    fable_u32 destinationRowPitch,
    fable_u32 width,
    fable_u32 height)
{
    const fable_u32 destinationBytesPerPixel =
        textureFormat == 0x15 ? 4 : (
            textureFormat == 0x19 ? 2 : 0);
    if (
        source == 0 ||
        destination == 0 ||
        destinationBytesPerPixel == 0 ||
        sourceRowPitch < width * 3 ||
        destinationRowPitch < width * destinationBytesPerPixel)
    {
        return false;
    }

    for (fable_u32 y = 0; y < height; ++y)
    {
        const fable_u8* sourcePixel =
            source + y * sourceRowPitch;
        fable_u8* destinationPixel =
            destination + y * destinationRowPitch;
        for (fable_u32 x = 0; x < width; ++x)
        {
            if (textureFormat == 0x15)
            {
                destinationPixel[0] = sourcePixel[0];
                destinationPixel[1] = sourcePixel[1];
                destinationPixel[2] = sourcePixel[2];
                destinationPixel[3] = 0xFF;
            }
            else
            {
                const fable_u16 pixel =
                    static_cast<fable_u16>(
                        0x8000 |
                        ((sourcePixel[2] & 0xF8) << 7) |
                        ((sourcePixel[1] & 0xF8) << 2) |
                        (sourcePixel[0] >> 3));
                destinationPixel[0] =
                    static_cast<fable_u8>(pixel);
                destinationPixel[1] =
                    static_cast<fable_u8>(pixel >> 8);
            }
            sourcePixel += 3;
            destinationPixel += destinationBytesPerPixel;
        }
    }
    return true;
}
