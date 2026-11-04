#pragma once

// Retail STLport 4.x __unguarded_linear_insert body (stl/_algo.c), apart from
// __linear_insert's header, which calls it out of line.
// <pair<unsigned long, CDefinitionManager::CDefClassInfo>*,
// CKeyPairCompareLess>: 0x009B76F0.

#include "fable_stlport_linear_insert.h"

namespace _STL
{
    // Shift the larger elements up one place until the value's slot is found;
    // a smaller-or-equal element below always exists (the caller checked the
    // first), so the walk needs no bound.
    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
    {
        _RandomAccessIter __next = __last;
        --__next;
        while (__comp(__val, *__next))
        {
            *__last = *__next;
            __last = __next;
            --__next;
        }
        *__last = __val;
    }
}
