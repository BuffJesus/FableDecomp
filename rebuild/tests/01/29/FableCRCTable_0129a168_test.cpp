#include "fable_crc.h"
#include <stdio.h>

int main()
{
    for (unsigned n = 0; n < 256; ++n)
    {
        fable_u32 value = n;
        for (unsigned bit = 0; bit < 8; ++bit)
            value = (value >> 1) ^ ((value & 1) ? 0xEDB88320u : 0);
        if (g_FableCRCTable_0129A168[n] != value) return 1;
    }
    puts("FABLE_CRC_TABLE_PASS cases=256");
    return 0;
}
