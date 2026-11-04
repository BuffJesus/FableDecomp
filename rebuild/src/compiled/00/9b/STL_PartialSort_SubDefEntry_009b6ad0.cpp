// _STL::__partial_sort<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B6AD0, from
// __introsort_loop 0x009B7DD0 when the depth limit runs out. __pop_heap
// expands inline; __make_heap 0x009B3C00 and sort_heap 0x009B5AF0 stay calls.
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
template<class I,class T,class C> void FABLE_FASTCALL __partial_sort(I first,I middle,I last,T*,C comp) {
 __make_heap(first,middle,comp,(T*)0,(int*)0);
 for(I i=middle;i<last;++i)if(comp(*i,*first))__pop_heap(first,middle,i,T(*i),comp,(int*)0);
 sort_heap(first,middle,comp);
}
}
typedef _STL::pair<unsigned long,CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Comp;
template void FABLE_FASTCALL _STL::__partial_sort(Entry*,Entry*,Entry*,Entry*,Comp);
