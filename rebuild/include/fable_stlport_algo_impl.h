#pragma once

// Retail STLport 4.x sort bodies (stl/_algo.c), opt-in like
// fable_stlport_vector_impl.h: fixtures stub the out-of-line pieces with
// explicit specializations, which must precede any implicit instantiation.
// Instantiations: sort <pair<CCharString, CSymbolInfo>*> 0x009E7E40 (final
// insertion pass expanded), <pair<unsigned long, CDefClassInfo>*> 0x009B9490
// (final insertion pass out of line, 0x009B91F0). __introsort_loop's body is
// in fable_stlport_introsort.h, so a sort fixture can stub it.

#include "fable_stlport_algo.h"

namespace _STL
{
    // Introsort down to runs of 16, then one insertion-sort pass over the whole range.
    template <class _RandomAccessIter, class _Compare>
    void FABLE_FASTCALL sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
    {
        if (__first != __last)
        {
            __introsort_loop(__first, __last, __value_type_of(__first), __lg(__last - __first) * 2, __comp);
            __final_insertion_sort(__first, __last, __comp);
        }
    }
}
