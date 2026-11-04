#pragma once

// Retail STLport 4.x copy / copy_backward for non-trivially-assignable
// element types (stl/_algobase.h): copy() dispatches to the counted
// random-access loops, which retail keeps out of line (fastcall; the tag and
// the null _Distance* are pushed). <pair<CCharString,
// CSymbolMap::CSymbolInfo>*>: __copy 0x009E6600, __copy_backward 0x009E6410.
// Bodies are visible to callers, as in STLport's headers.

#include <stddef.h>
#include "rebuild_abi.h"

#include "fable_stlport_vector.h"

// Shader layout vectors retain the renderer TU's fastcall fill instantiation
// (0098C120). Other recovered units use the default calling convention.
#ifndef FABLE_STLP_FILL_CALL
#define FABLE_STLP_FILL_CALL
#endif

namespace _STL
{
    template <class _RandomAccessIter, class _OutputIter, class _Distance>
    FABLE_UNIT_INLINE _OutputIter FABLE_FASTCALL __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                             _OutputIter __result, const random_access_iterator_tag&, _Distance*)
    {
        for (_Distance __n = __last - __first; __n > 0; --__n)
        {
            *__result = *__first;
            ++__first;
            ++__result;
        }
        return __result;
    }

    template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
    inline _BidirectionalIter FABLE_FASTCALL __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                             _BidirectionalIter __result,
                                                             const random_access_iterator_tag&, _Distance*)
    {
        for (_Distance __n = __last - __first; __n > 0; --__n)
            *--__result = *--__last;
        return __result;
    }

    // Pointer ranges of class elements (STLport's __copy_ptrs /
    // __copy_backward_ptrs with __false_type, inline).
    template <class _InputIter, class _OutputIter>
    FABLE_UNIT_INLINE _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
    {
        return __copy(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
    }

    // STLport fill: a loop, and memset for the byte types (vector<char>::
    // _M_fill_insert 0x004110D0 expands these to rep stos).
    template <class _ForwardIter, class _Tp>
    inline void FABLE_STLP_FILL_CALL fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
    {
        for (; __first != __last; ++__first)
            *__first = __val;
    }
    inline void fill(char* __first, char* __last, const char& __val)
    {
        unsigned char __tmp = __val;
        memset(__first, __tmp, __last - __first);
    }
    inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
    {
        unsigned char __tmp = __val;
        memset(__first, __tmp, __last - __first);
    }

    // STLport __copy_backward_ptrs for trivially assignable elements: one memmove.
    template <class _Tp>
    inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
    {
        const ptrdiff_t _Num = __last - __first;
        if (_Num > 0)
            memmove(__result - _Num, __first, _Num * sizeof(_Tp));
        return __result - _Num;
    }

    // STLport fill_n (inline; the POD vector::_M_insert_overflow 0x009B4BD0
    // runs it as a counted loop that re-reads the value each time).
    template <class _OutputIter, class _Size, class _Tp>
    inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
    {
        for (; __n > 0; --__n, ++__first)
            *__first = __val;
        return __first;
    }
    // The byte forms go through fill (memset).
    inline char* fill_n(char* __first, size_t __n, const char& __val)
    {
        fill(__first, __first + __n, __val);
        return __first + __n;
    }
    inline unsigned char* fill_n(unsigned char* __first, size_t __n, const unsigned char& __val)
    {
        fill(__first, __first + __n, __val);
        return __first + __n;
    }

    // STLport's POD uninitialized fill: plain fill_n.
    template <class _ForwardIter, class _Size, class _Tp>
    inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
    {
        return fill_n(__first, __n, __x);
    }

    // Class elements are assigned back to front by the counted loop
    // (<CLipSyncDataBankEntry*>: __copy_backward 0x00A43720).
    template <class _Tp>
    inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
    {
        return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
    }

    template <class _BidirectionalIter1, class _BidirectionalIter2>
    inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                             _BidirectionalIter2 __result)
    {
        return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (ptrdiff_t*)0);
    }
}
