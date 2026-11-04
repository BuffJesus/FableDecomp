#pragma once

// Retail STLport map/tree members VC7.1 keeps out of line (bodies the STLport
// headers carry). Opt-in, like fable_stlport_vector_impl.h: fixtures stub
// these members as explicit specializations, which must come before any
// implicit instantiation. map<CCharString, long>: operator[] 0x00519230.

#include "fable_stlport_tree.h"

namespace _STL
{
    // The value under the key, inserted default-constructed (with the lower
    // bound as the hint) when the key is new.
    template <class _Key, class _Tp, class _Compare, class _Alloc>
    _Tp& map<_Key, _Tp, _Compare, _Alloc>::operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
}
