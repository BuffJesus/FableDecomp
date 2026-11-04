#pragma once

// Retail STLport 4.x __insertion_sort body (stl/_algo.c), kept apart from
// sort's so sort's fixture can stub it; its own fixtures stub __linear_insert.
// Retail's form takes no value-type tag. Instantiations:
// <pair<CCharString, CSymbolInfo>*> 0x009E7590, <pair<unsigned long,
// CDefClassInfo>*> 0x009B8F70.

#include "fable_stlport_algo.h"

namespace _STL
{
    // Grow the sorted prefix one element at a time.
    template <class _RandomAccessIter, class _Compare>
    void FABLE_FASTCALL __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
    {
        typedef typename iterator_traits<_RandomAccessIter>::value_type _Tp;
        if (__first == __last)
            return;
        for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
            __linear_insert(__first, __i, _Tp(*__i), __comp);
    }
}
