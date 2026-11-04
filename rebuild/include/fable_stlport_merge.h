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

// Modified for native VC7.1 pointer iterators and recovered fastcall boundaries.
// Native stable-merge family begins at 00B86140; later STLport _algo.c is
// supporting algorithm evidence, not an original retail symbol oracle.
#include "fable_stlport_algo.h"
#include "fable_stlport_vector.h"
namespace _STL {
template<class I,class T,class C,class D> I FABLE_FASTCALL __lower_bound(I,I,const T&,C,D*);
template<class I,class T,class C,class D> I FABLE_FASTCALL __upper_bound(I,I,const T&,C,D*);
template<class I,class D> I FABLE_FASTCALL __rotate(I,I,I,D*,random_access_iterator_tag);
template<class I,class J> J FABLE_FASTCALL swap_ranges(I,I,J);
template<class I,class D,class C> void FABLE_FASTCALL __merge_without_buffer(I,I,I,D,D,C);
}
