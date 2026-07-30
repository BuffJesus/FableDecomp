#pragma once

#include "rebuild_abi.h"

extern fable_u32 g_FableCRCTable_0129A168[256];

fable_u32 FABLE_FASTCALL
FableCRC_Calc_004014A0(
    fable_u32 seed,
    const void* data,
    fable_u32 byteCount);
