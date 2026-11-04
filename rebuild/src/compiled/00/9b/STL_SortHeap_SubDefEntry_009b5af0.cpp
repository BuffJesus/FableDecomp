// _STL::sort_heap<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B5AF0, from __partial_sort
// 0x009B6AD0; __pop_heap expands inline.
#include "fable_def_class_base.h"
#include "fable_stlport_heap.h"
namespace _STL {
// Shared with the other heap unit; guarded so one translation unit can hold both.
#ifndef FABLE_SUBDEF_POP_HEAP_BODY
#define FABLE_SUBDEF_POP_HEAP_BODY
template<class I,class T,class C,class D> void FABLE_FASTCALL __pop_heap(I first,I last,I result,T value,C comp,D*) {
 *result=*first;
 __adjust_heap(first,D(0),D(last-first),value,comp);
}
#endif
template<class I,class C> void FABLE_FASTCALL sort_heap(I first,I last,C comp) {
 typedef typename iterator_traits<I>::value_type T;
 while(last-first>1){__pop_heap(first,last-1,last-1,T(*(last-1)),comp,(int*)0);--last;}
}
}
typedef _STL::pair<unsigned long,CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Comp;
template void FABLE_FASTCALL _STL::sort_heap(Entry*,Entry*,Comp);
