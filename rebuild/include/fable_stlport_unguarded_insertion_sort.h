#pragma once

// STLport insertion pass after a sorted prefix has supplied a sentinel.
// CRC/global-index pairs: retail 0x009B2D40.
#include "fable_stlport_unguarded_linear_insert.h"
namespace _STL
{
    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last,
                                                       _Tp*, _Compare __comp)
    {
        for (_RandomAccessIter __i = __first; __i != __last; ++__i)
            __unguarded_linear_insert(__i, _Tp(*__i), __comp);
    }
}
