#pragma once

#include "rebuild_abi.h"

extern fable_u32 g_FableCRCTable_0129A168[256];

fable_u32 FABLE_FASTCALL
FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount);

// Lionhead's CCRC (FableWin ?Calc@CCRC@@SIKKPBXK@Z): the table CRC above as
// an inline static. Retail's out-of-line copy is 0x004014A0 (landed as
// FableCRC_Calc_004014A0, which GetCRC calls); where the body is visible and
// the length constant VC7.1 expands and unrolls it (CDefStringTable
// 0x009D4DB0 over a 16-byte SYSTEMTIME).
class CCRC
{
public:
    // Retail 0x00A00950: zero-seeded state-block CRC over a pointer and size.
    fable_u32 Calc(const void* data, fable_u32 byteCount) const;

    static fable_u32 FABLE_FASTCALL Calc(fable_u32 seed, const void* data, fable_u32 byteCount)
    {
        if (byteCount != 0)
        {
            const fable_u8* bytes = static_cast<const fable_u8*>(data);
            for (fable_u32 index = 0; index < byteCount; ++index)
                seed = g_FableCRCTable_0129A168[bytes[index] ^ (seed & 0xFF)] ^ (seed >> 8);
        }
        return seed;
    }
};
