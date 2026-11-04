#pragma once

// STLport's non-POD range helpers (stl/_uninitialized.h, stl/_construct.h),
// defined where the vector header declares them, as STLport's headers do.
// Retail keeps them out of line (fastcall: 0x009E6270 __uninitialized_copy,
// 0x009E62E0 __uninitialized_fill_n, 0x006FBCC0 __destroy_aux for
// pair<CCharString, CSymbolMap::CSymbolInfo>*). Their bodies must be visible
// to the callers: VC7.1 sees that none of them reads its tag argument and
// then lets the caller overlay every tag temporary on one live argument
// slot (vector::_M_insert_overflow 0x009E77E0 keeps a 4-byte frame).
// Units that construct in place include <new> for the throw() placement new.

#include "fable_stlport_vector.h"

namespace _STL
{
    // STLport's public uninitialized_fill_n for class elements: declared
    // here, its body defined by the unit that lands each instantiation
    // (<CCharString>: 0x00433504; <CLipSyncDataBankEntry>: 0x00A43670).
    template <class T> T* FABLE_FASTCALL uninitialized_fill_n(T* first, size_t count, const T& value);

    template <class _InputIter, class _ForwardIter>
    inline _ForwardIter FABLE_FASTCALL __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                     _ForwardIter __result, const __false_type&)
    {
        _ForwardIter __cur = __result;
        {   // STLport's _STLP_TRY block (exceptions are off in retail)
            for (; __first != __last; ++__first, ++__cur)
                _Construct(&*__cur, *__first);
            return __cur;
        }
    }

    template <class _ForwardIter, class _Size, class _Tp>
    inline _ForwardIter FABLE_FASTCALL __uninitialized_fill_n(_ForwardIter __first, _Size __n,
                                                       const _Tp& __x, const __false_type&)
    {
        _ForwardIter __cur = __first;
        {   // STLport's _STLP_TRY block (exceptions are off in retail)
            for (; __n > 0; --__n, ++__cur)
                _Construct(&*__cur, __x);
            return __cur;
        }
    }

    template <class _ForwardIter>
    inline void FABLE_FASTCALL __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            _Destroy(&*__first);
    }
}
