#pragma once

// Retail STLport 4.x __introsort_loop body (stl/_algo.c), kept apart from
// sort's (fable_stlport_algo_impl.h) so a unit that instantiates sort does not
// also emit it (sort's fixture stubs it). Instantiations:
// <pair<CCharString, CSymbolInfo>*> 0x009E7D30, <pair<unsigned long,
// CDefClassInfo>*> 0x009B9320.

#include "fable_stlport_algo.h"

namespace _STL
{
    // Quicksort on the median of three until runs are at most 16 long; when the
    // depth limit runs out, heap-sort what is left.
    template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
    void FABLE_FASTCALL __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                         _Size __depth_limit, _Compare __comp)
    {
        while (__last - __first > __stl_threshold)
        {
            if (__depth_limit == 0)
            {
                partial_sort(__first, __last, __last, __comp);
                return;
            }
            --__depth_limit;
            _RandomAccessIter __cut =
                __unguarded_partition(__first, __last,
                                      _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1),
                                                   __comp)),
                                      __comp);
            __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
            __last = __cut;
        }
    }
}
