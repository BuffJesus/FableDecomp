// Native STLport PartialSort for CBankFile::InitBlockOrder entry indices at 0x009D4620.
#include "fable_game_text_bank.h"
#include "fable_stlport_heap.h"
namespace _STL {
#ifndef FABLE_INDEX_POP_HEAP_BODY
#define FABLE_INDEX_POP_HEAP_BODY
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
typedef CBankFile::CompareDataOffset Comp;
template void FABLE_FASTCALL _STL::__partial_sort(unsigned long*,unsigned long*,unsigned long*,unsigned long*,Comp);
