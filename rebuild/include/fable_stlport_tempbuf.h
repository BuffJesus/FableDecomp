#pragma once
/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

// Modified for the recovered retail VC7.1 ABI: standalone allocation provider,
// pointer iterators, and nontrivial default construction for CSortElement.
// Layout/allocation cross-checked against retail 00B84E20 / 00B86710.
// Reference: android.googlesource.com/platform/external/stlport/+/e64e7c849b0b659d3e198e4efaed563601e554be/stl/_tempbuf.h
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <new>
#include "rebuild_abi.h"
namespace _STL {
template<class I,class T> class _Temporary_buffer {
 ptrdiff_t _M_original_len;
 ptrdiff_t _M_len;
 T* _M_buffer;
 void _M_allocate_buffer();
public:
 FABLE_UNIT_INLINE _Temporary_buffer(I first,I last):_M_len(last-first){_M_allocate_buffer();if(_M_len>0)for(ptrdiff_t i=0;i<_M_len;++i)new(_M_buffer+i)T(*first);}
 ~_Temporary_buffer(){for(ptrdiff_t i=0;i<_M_len;++i)(_M_buffer+i)->~T();free(_M_buffer);}
 ptrdiff_t size()const{return _M_len;}
 ptrdiff_t requested_size()const{return _M_original_len;}
 T* begin(){return _M_buffer;}
};
template<class I,class C> void FABLE_FASTCALL __inplace_stable_sort(I,I,C);
template<class I,class P,class D,class C> void FABLE_FASTCALL __stable_sort_adaptive(I,I,P,D,C);
}
