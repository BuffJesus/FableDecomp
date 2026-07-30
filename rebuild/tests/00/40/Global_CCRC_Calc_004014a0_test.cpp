#include <stdio.h>

#include "fable_crc.h"

fable_u32 g_FableCRCTable_0129A168[256];

static void BuildTable()
{
    for (fable_u32 index = 0; index != 256; ++index)
    {
        fable_u32 value = index;
        for (fable_u32 bit = 0; bit != 8; ++bit)
        {
            value = (value & 1)
                ? ((value >> 1) ^ 0xEDB88320)
                : (value >> 1);
        }
        g_FableCRCTable_0129A168[index] = value;
    }
}

int main()
{
    BuildTable();

    if (FableCRC_Calc_004014A0(0x12345678, 0, 0) != 0x12345678)
        return 1;

    static const fable_u8 fonts[] = {'f', 'o', 'n', 't', 's'};
    if (FableCRC_Calc_004014A0(0, fonts, sizeof(fonts)) != 0xB5211FE6)
        return 2;

    static const fable_u8 names[] = {
        'n', 'a', 'm', 'e', 's', '.', 'b', 'i', 'n'
    };
    if (
        FableCRC_Calc_004014A0(
            0x12345678,
            names,
            sizeof(names)) != 0x5B41316F)
    {
        return 3;
    }

    puts("FABLETLC_CRC_CALC_BEHAVIOR PASS");
    return 0;
}
