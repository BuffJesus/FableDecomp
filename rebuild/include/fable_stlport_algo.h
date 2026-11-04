#pragma once

// Retail STLport 4.x sort (stl/_algo.h, _algo.c). The out-of-line pieces are
// declared here and defined per instantiation in the landings that
// reconstruct them; free functions are __fastcall in retail (first two
// arguments in ecx/edx, the rest pushed, callee pops). The inline glue
// (__lg, __final_insertion_sort, __unguarded_insertion_sort) is here.
//
// <pair<CCharString, CSymbolMap::CSymbolInfo>*, CCompareString<CSymbolInfo> >:
//   sort                              0x009E7E40
//   __introsort_loop                  0x009E7D30
//   __insertion_sort                  0x009E7590
//   __unguarded_insertion_sort_aux    0x009E6D10
//   __unguarded_partition             0x009E6C80
//   __linear_insert                   0x009E6FA0
//   __partial_sort                    0x009E7B40

#include "rebuild_abi.h"

namespace _STL
{
    // Original std::find over input iterators (CPlayer::HasMode, 00634160).
    template<class I, class T>
    inline I FABLE_FASTCALL find(I first, I last, const T& value)
    {
        while (first != last && *first != value)
            ++first;
        return first;
    }

    enum { __stl_threshold = 16 };

    // Only the pointer case is modelled.
    template <class _Iterator>
    struct iterator_traits;
    template <class _Tp>
    struct iterator_traits<_Tp*>
    {
        typedef _Tp value_type;
    };

    // floor(log2(n)); inline in retail's sort.
    template <class _Size>
    inline _Size __lg(_Size __n)
    {
        _Size __k;
        for (__k = 0; __n != 1; __n >>= 1)
            ++__k;
        return __k;
    }

    template <class _RandomAccessIter, class _Compare>
    void FABLE_FASTCALL sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);

    // Default-comparator overload and stable-sort helper used by the retail
    // subprimitive dispatcher (00B868C0 / 00B86710).
    template <class _RandomAccessIter>
    void FABLE_FASTCALL sort(_RandomAccessIter __first, _RandomAccessIter __last);
    template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
    void FABLE_FASTCALL __stable_sort(_RandomAccessIter, _RandomAccessIter, _Tp*, _Distance*, _Compare);

    template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
    void FABLE_FASTCALL __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                         _Size __depth_limit, _Compare __comp);

    // Retail's form takes no value-type tag (0x009E7590 pops only the comparator).
    template <class _RandomAccessIter, class _Compare>
    void FABLE_FASTCALL __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);

    // Insert *__last (passed by value) into the sorted [__first, __last).
    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val,
                                        _Compare __comp);

    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last,
                                                       _Tp*, _Compare __comp);

    template <class _RandomAccessIter, class _Tp, class _Compare>
    _RandomAccessIter FABLE_FASTCALL __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last,
                                                           _Tp __pivot, _Compare __comp);

    template <class _RandomAccessIter, class _Tp, class _Compare>
    void FABLE_FASTCALL __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle,
                                       _RandomAccessIter __last, _Tp*, _Compare __comp);

    // Median of three by reference; inline in retail's __introsort_loop, except
    // <unsigned long, CBankFile::CompareDataOffset> 0x009D2E40 (out of line, fastcall).
    template <class _Tp, class _Compare>
    inline const _Tp& FABLE_FASTCALL __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
    {
        if (__comp(__a, __b))
            if (__comp(__b, __c))
                return __b;
            else if (__comp(__a, __c))
                return __c;
            else
                return __a;
        else if (__comp(__a, __c))
            return __a;
        else if (__comp(__b, __c))
            return __c;
        else
            return __b;
    }

    template <class _Tp>
    inline _Tp* __value_type_of(_Tp*)
    {
        return (_Tp*)0;
    }

    // Inline in retail's __introsort_loop (depth exhausted: heap-sort the range).
    template <class _RandomAccessIter, class _Compare>
    inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                             _Compare __comp)
    {
        __partial_sort(__first, __middle, __last, __value_type_of(__first), __comp);
    }

    template <class _RandomAccessIter, class _Compare>
    inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
    {
        __unguarded_insertion_sort_aux(__first, __last, __value_type_of(__first), __comp);
    }

    // Out of line in STLport (_algo.c) and in retail for
    // <pair<unsigned long, CDefClassInfo>*> (0x009B91F0); sort 0x009E7E40
    // expands it for <pair<CCharString, CSymbolInfo>*>, which VC7.1 only does
    // when it was instantiated before sort in the unit.
    template <class _RandomAccessIter, class _Compare>
    void FABLE_FASTCALL __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
    {
        if (__last - __first > __stl_threshold)
        {
            __insertion_sort(__first, __first + __stl_threshold, __comp);
            __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
        }
        else
            __insertion_sort(__first, __last, __comp);
    }
}
