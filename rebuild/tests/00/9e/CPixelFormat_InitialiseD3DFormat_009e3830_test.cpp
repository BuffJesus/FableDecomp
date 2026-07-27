#include <stdio.h>

#include "fable_texture_lifecycle.h"
#include "../../../src/compiled/01/29/CPixelFormat_Table_0129ba40.cpp"

int main()
{
    CPixelFormatByteLengthView format;
    for (
        fable_u32 index = 0;
        index < FABLE_PIXEL_FORMAT_COUNT;
        ++index)
    {
        format.value00 = 0xcccccccc;
        format.Initialise(
            static_cast<fable_u32>(
                g_FablePixelFormatTable[index].d3dFormat00));
        if (format.value00 != index)
        {
            printf(
                "FABLETLC_PIXEL_FORMAT_INITIALISE FAIL index=%lu got=%lu\n",
                index,
                format.value00);
            return 1;
        }
    }

    format.value00 = 0;
    format.Initialise(0x7fffffff);
    if (format.value00 != static_cast<fable_u32>(-1))
    {
        printf(
            "FABLETLC_PIXEL_FORMAT_INITIALISE FAIL unknown=%lu\n",
            format.value00);
        return 2;
    }

    format.Initialise(102);
    if (format.value00 != 45)
    {
        printf(
            "FABLETLC_PIXEL_FORMAT_INITIALISE FAIL duplicate=%lu\n",
            format.value00);
        return 3;
    }

    printf("FABLETLC_PIXEL_FORMAT_INITIALISE PASS\n");
    return 0;
}
