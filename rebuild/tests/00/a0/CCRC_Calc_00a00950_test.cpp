#include "fable_crc.h"
#include <stdio.h>

#ifndef CRC_EXTERNAL_TABLE
fable_u32 g_FableCRCTable_0129A168[256];
static void InitTable()
{
    for (unsigned n = 0; n < 256; ++n)
    {
        fable_u32 value = n;
        for (unsigned bit = 0; bit < 8; ++bit)
            value = (value >> 1) ^ ((value & 1) ? 0xEDB88320u : 0);
        g_FableCRCTable_0129A168[n] = value;
    }
}
#else
static void InitTable() {}
#endif

static fable_u32 Reference(const fable_u8* data, unsigned size)
{
    fable_u32 value = 0;
    for (unsigned i = 0; i < size; ++i)
    {
        value ^= data[i];
        for (unsigned bit = 0; bit < 8; ++bit)
            value = (value >> 1) ^ ((value & 1) ? 0xEDB88320u : 0);
    }
    return value;
}

int main()
{
    InitTable();
    CCRC crc;
    fable_u8 bytes[1024];
    for (unsigned i = 0; i < sizeof(bytes); ++i)
        bytes[i] = (fable_u8)((i * 37 + 11) & 255);
    if (crc.Calc(0, 0) != 0) return 1;
    const unsigned sizes[] = {1, 2, 3, 4, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128, 255, 1024};
    for (unsigned i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i)
        if (crc.Calc(bytes, sizes[i]) != Reference(bytes, sizes[i])) return 2;
    puts("CCRC_INSTANCE_PASS cases=17");
    return 0;
}
