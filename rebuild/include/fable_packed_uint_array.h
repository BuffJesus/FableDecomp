#pragma once

// Hand-owned retail CPackedUIntArray constructor model (0x00A629C0).
#include "rebuild_abi.h"

class CPackedUIntArray
{
public:
    CPackedUIntArray();
    unsigned long CalculateBias(void* source, unsigned long sourceBits, long stride, unsigned long count);
    unsigned long CountBits(void* source, unsigned long sourceBits, long stride, unsigned long count);
    void PackArray(void* source, unsigned long sourceBits, long stride, unsigned long wordCount);
    void Initialise(void* source, unsigned long sourceBits, long stride, unsigned long count);
    unsigned long Sizeof() const;
    unsigned long operator[](unsigned long index) const;
    ~CPackedUIntArray();
    unsigned long* PackedInts;
    unsigned long Size;
    unsigned char Bits;
    unsigned char Padding09[3];
    unsigned long Bias;
};
FABLE_STATIC_ASSERT(sizeof(CPackedUIntArray) == 0x10);

// Retail mask words for widths zero through 32.
extern unsigned long g_PackedUIntMasks_013979E8[33];
