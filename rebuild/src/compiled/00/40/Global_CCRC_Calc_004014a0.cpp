#include "fable_crc.h"

fable_u32 FABLE_FASTCALL
FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount)
{
    if (byteCount != 0)
    {
        const fable_u8* bytes =
            static_cast<const fable_u8*>(data);
        for (fable_u32 index = 0; index < byteCount; ++index)
        {
            seed =
                g_FableCRCTable_0129A168[
                    bytes[index] ^ (seed & 0xFF)] ^
                (seed >> 8);
        }
    }
    return seed;
}
