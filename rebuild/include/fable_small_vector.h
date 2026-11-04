#pragma once
#include "rebuild_abi.h"

// Retail CSmallVector<T,8> holds an array-new allocation and a length byte.
// Clear uses the allocation cookie when destroying nontrivial elements.
template<class T, unsigned Bits> class CSmallVector {
public:
    // Inline in retail (member-initialised first in CBankFileEntryUpdateData's
    // constructor, CBankFile::ReadEntryList 009CFBC0).
    CSmallVector() : Data(0), Size(0) {}
    T* Data;
    signed char Size;   // Ego_r: protected 1-byte signed (DIA btInt); retail sign-extends it (0x00A76500)
    void Clear();
    void Allocate(unsigned long count);
    void Copy(const CSmallVector& other);
};
FABLE_STATIC_ASSERT(sizeof(CSmallVector<unsigned long, 8>) == 8);
