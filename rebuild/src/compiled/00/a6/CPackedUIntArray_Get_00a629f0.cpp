#include "fable_packed_uint_array.h"
unsigned long CPackedUIntArray::operator[](unsigned long index) const
{
    if (!Bits) return Bias;
    unsigned long bit = Bits * index;
    unsigned long word = bit / 32;
    unsigned long shift = bit - word * 32;
    const unsigned long* data = PackedInts + word;
    unsigned long value = *data >> shift;
    if (shift + Bits > 32) value |= data[1] << (32 - shift);
    return (value & g_PackedUIntMasks_013979E8[Bits]) + Bias;
}

// Literal table at retail 013979e8, verified against the executable.
unsigned long g_PackedUIntMasks_013979E8[33] = {
    0x00000000ul, 0x00000001ul, 0x00000003ul, 0x00000007ul, 0x0000000ful, 0x0000001ful, 0x0000003ful, 0x0000007ful, 0x000000fful, 0x000001fful, 0x000003fful, 0x000007fful, 0x00000ffful, 0x00001ffful, 0x00003ffful, 0x00007ffful, 0x0000fffful, 0x0001fffful, 0x0003fffful, 0x0007fffful, 0x000ffffful, 0x001ffffful, 0x003ffffful, 0x007ffffful, 0x00fffffful, 0x01fffffful, 0x03fffffful, 0x07fffffful, 0x0ffffffful, 0x1ffffffful, 0x3ffffffful, 0x7ffffffful, 0xfffffffful
};
