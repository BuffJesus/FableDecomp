#pragma once
#include "fable_stlport_algo.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I,D,D,T,C);
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I,D,D,T,C);
template<class I,class T,class C,class D> void FABLE_FASTCALL __pop_heap(I,I,I,T,C,D*);
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I,I,C,T*,D*);
template<class I,class C> void FABLE_FASTCALL sort_heap(I,I,C);
}
