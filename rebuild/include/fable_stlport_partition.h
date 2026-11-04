#pragma once

// STLport Hoare partition. The pivot is copied so swaps cannot change it.
// Unsigned CRC/global-index pairs instantiate this at retail 0x009B3890.
#include "fable_stlport_algo.h"
namespace _STL
{
    template <class _RandomAccessIter, class _Tp, class _Compare>
    _RandomAccessIter FABLE_FASTCALL __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                           _Tp __pivot, _Compare __comp)
    {
        for (;;)
        {
            while (__comp(*__first, __pivot)) ++__first;
            --__last;
            while (__comp(__pivot, *__last)) --__last;
            if (!(__first < __last)) return __first;
            _Tp __tmp = *__first;
            *__first = *__last;
            *__last = __tmp;
            ++__first;
        }
    }
}
