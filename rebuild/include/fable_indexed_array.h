#pragma once
#include "fable_carray.h"
#include "fable_stlport_tree.h"
// Ego_r CIndexedArray<T>: value array followed by a value-to-index map.
// Retail STLport containers occupy12 bytes each (donor:16 bytes each).
template<class T> class CIndexedArray {
public:
    void push_back(const T& value);
    CArrayVector<T> Array;
    _STL::map<T,long> IndexMap;
};
FABLE_STATIC_ASSERT(sizeof(CIndexedArray<CCharString>)==24);
