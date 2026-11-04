#include "fable_crc.h"

fable_u32 CCRC::Calc(const void* data, fable_u32 byteCount) const
{
    fable_u32 seed = 0;
    if (byteCount == 0) return seed;
    const fable_u8* bytes = static_cast<const fable_u8*>(data);
    for (fable_u32 index = 0; index < byteCount; ++index)
        seed = g_FableCRCTable_0129A168[bytes[index] ^ (seed & 0xff)] ^ (seed >> 8);
    return seed;
}
