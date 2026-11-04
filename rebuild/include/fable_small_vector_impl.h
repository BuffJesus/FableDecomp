#pragma once

// CSmallVector member bodies (retail <CCharString,8>: Allocate 0x009D2160, Copy
// 0x009D21D0). Opt-in like the STLport *_impl.h headers: a unit that sees them lets
// VC7.1 know the calls keep `this` to themselves (CBankFile::ReadEntryList 009CFBC0
// caches the element pointer across later calls only with Allocate's body visible).
#include "fable_small_vector.h"

template<class T, unsigned Bits>
void CSmallVector<T, Bits>::Allocate(unsigned long count)
{
    Clear();
    Data = new T[count];
    Size = static_cast<unsigned char>(count);
}

template<class T, unsigned Bits>
void CSmallVector<T, Bits>::Copy(const CSmallVector& other)
{
    // Retail sign-extends each length-byte read, including the loop reload.
    Allocate(static_cast<signed char>(other.Size));
    for (unsigned long i = 0; i < static_cast<signed char>(other.Size); ++i)
        Data[i] = other.Data[i];
}
