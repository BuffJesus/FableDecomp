// CBasicString<char>::ExtendData(unsigned long) @ 0x009A01C0 (FableWin
// ?ExtendData@?$CBasicString@D@@IAEXK@Z; Append 0x009A04E0).
//
// Retail's register and argument-slot choices come from Lionhead's helper
// structure (FableWin names): AllocateDataFromStringLength picks the buffer
// size (aligned, rounded up to a power of two when fast-extending, chosen per
// branch into its own local) and calls AllocateDataFromDataLength;
// ClearStringEnd and CopyAndTerminate fill the new buffer. The bit search
// increments its own argument. Both size helpers must be force-inlined.
#include "fable_basic_string.h"

#include <new>
#include <string.h>

// Index of the highest set bit (-1 for zero): a branch-free binary search.
static __forceinline long HighestBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    value++;
    long bit = ((long)(0x10000 - value) >> 31) & 16;
    bit += ((long)((1 << (bit + 8)) - value) >> 31) & 8;
    bit += ((long)((1 << (bit + 4)) - value) >> 31) & 4;
    bit += ((long)((1 << (bit + 2)) - value) >> 31) & 2;
    return bit + (((long)((1 << (bit + 1)) - value) >> 31) & 1);
}

// Smallest power of two >= value (zero stays zero).
static __forceinline unsigned long RoundUpToPowerOfTwo(unsigned long value)
{
    if (value == 0)
        return value;
    return 1 << HighestBit(value * 2 - 1);
}

template <class T>
inline unsigned long __fastcall CBasicString<T>::Align(unsigned long length)
{
    return (length + 3) & ~3;
}

template <class T>
inline void __fastcall CBasicString<T>::CopyAndTerminate(T* dest, const T* src, unsigned long length)
{
    memcpy(dest, src, length);
    dest[length] = 0;
}

// Zero the buffer from after the terminator up to the next 4-byte boundary.
template <class T>
inline void CBasicString<T>::ClearStringEnd(T* data, unsigned long length)
{
    unsigned long clearFrom = length + 1;
    memset(data + clearFrom, 0, Align(clearFrom) - clearFrom);
}

template <class T>
inline void CBasicString<T>::AllocateDataFromDataLength(unsigned long dataLength, T** data)
{
    *data = new T[dataLength];
}

// Allocate a buffer for a string of the given length; returns its size.
template <class T>
__forceinline unsigned long CBasicString<T>::AllocateDataFromStringLength(unsigned long length, T** data)
{
    unsigned long alignedLength = Align(length + 1);
    unsigned long dataLength;
    if (UseFastExtend)
        dataLength = RoundUpToPowerOfTwo(alignedLength);
    else
        dataLength = alignedLength;
    AllocateDataFromDataLength(dataLength, data);
    return dataLength;
}

// Grow the buffer so it holds a string of the given length, keeping the text.
template <class T>
void CBasicString<T>::ExtendData(unsigned long length)
{
    if (Align(length + 1) > DataLength)
    {
        T* data;
        unsigned long dataLength = AllocateDataFromStringLength(length, &data);
        ClearStringEnd(data, StringLength);
        CopyAndTerminate(data, PData, StringLength);
        if (PData != 0)
        {
            delete[] PData;
            PData = 0;
        }
        DataLength = dataLength;
        PData = data;
    }
}

template void CBasicString<char>::ExtendData(unsigned long length);