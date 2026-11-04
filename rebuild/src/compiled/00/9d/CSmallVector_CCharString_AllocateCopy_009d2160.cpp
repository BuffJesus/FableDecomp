#include <new>
#include "fable_string.h"
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

template void CSmallVector<CCharString, 8>::Allocate(unsigned long);
template void CSmallVector<CCharString, 8>::Copy(const CSmallVector&);
