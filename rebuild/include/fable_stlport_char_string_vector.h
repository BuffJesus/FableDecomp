#pragma once
// Non-POD string-vector operations used by the binary persistence reader.
// Retail keeps these instantiations out of line in the size-optimised unit.
#include "fable_string.h"
#include "fable_stlport_vector.h"
namespace _STL {
    template <> CCharString* vector<CCharString>::erase(CCharString*, CCharString*);
    template <> void vector<CCharString>::_M_fill_insert(CCharString*, size_t, const CCharString&);
    template <class T> T* FABLE_FASTCALL uninitialized_fill_n(T* first, size_t count, const T& value);
    template <class T> void FABLE_FASTCALL _Destroy_range(T* first, T* last);
}
