#pragma once

#include <stddef.h>

#include "rebuild_abi.h"

struct FableVectorMapEntry
{
    fable_u32 key;
    fable_u32 value;
};

struct FableVectorMapStorage
{
    FableVectorMapEntry* LowerBound(
        const fable_u32& key);
    FableVectorMapEntry* begin_;
    FableVectorMapEntry* end_;
};

class FableVectorMapRecovered
{
public:
    fable_u32 LookupValue(fable_u32 key);

    fable_u8 unknown00_[0xD0];
    FableVectorMapStorage entries_;
};

FABLE_STATIC_ASSERT(offsetof(FableVectorMapRecovered, entries_) == 0xD0);
FABLE_STATIC_ASSERT(offsetof(FableVectorMapStorage, end_) == 0x04);
