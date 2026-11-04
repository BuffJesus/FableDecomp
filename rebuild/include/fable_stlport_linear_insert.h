#pragma once

// Retail STLport 4.x __linear_insert / __unguarded_linear_insert bodies
// (stl/_algo.c), kept apart from __insertion_sort's header so that its
// landings (which call __linear_insert out of line) and fixtures do not see
// them. <pair<unsigned long, CDefinitionManager::CDefClassInfo>*,
// CKeyPairCompareLess>: __linear_insert 0x009B8C00, __unguarded_linear_insert
// 0x009B76F0, __copy_backward 0x009B7870. <pair<CCharString,
// CSymbolMap::CSymbolInfo>*>: 0x009E6FA0, 0x009E66F0, 0x009E6410.

#include "fable_stlport_algo.h"
#include "fable_stlport_algobase.h"

namespace _STL
{
    // Out of line in retail; the body is in fable_stlport_unguarded_linear_insert.h.
    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);

    // A value below the first moves everything up one place; otherwise walk
    // back from the end (the first element bounds the walk).
    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val,
                                        _Compare __comp)
    {
        if (__comp(__val, *__first))
        {
            copy_backward(__first, __last, __last + 1);
            *__first = __val;
        }
        else
            __unguarded_linear_insert(__last, __val, __comp);
    }
}
