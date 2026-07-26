#include <stdio.h>

#include "fable_texture_lifecycle.h"
#include "../../../src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp"

int main()
{
    CPixelFormatByteLengthView format;
    for (
        fable_u32 index = 0;
        index < FABLE_PIXEL_FORMAT_TABLE_COUNT;
        ++index)
    {
        format.value00 = index;
        if (
            format.GetColourDepth() !=
            static_cast<fable_u32>(
                g_FablePixelFormatTable[index].colourDepth08))
        {
            printf(
                "FABLETLC_PIXEL_FORMAT_COLOUR_DEPTH FAIL index=%lu\n",
                index);
            return 1;
        }
    }

    if (
        g_FablePixelFormatTable[0].d3dFormat00 != 20 ||
        g_FablePixelFormatTable[1].colourDepth08 != 32 ||
        g_FablePixelFormatTable[18].colourDepth08 != 128 ||
        g_FablePixelFormatTable[31].d3dFormat00 != 0x31545844 ||
        g_FablePixelFormatTable[46].colourDepth08 != -1)
    {
        printf("FABLETLC_PIXEL_FORMAT_COLOUR_DEPTH FAIL table\n");
        return 2;
    }

    printf("FABLETLC_PIXEL_FORMAT_COLOUR_DEPTH PASS\n");
    return 0;
}
