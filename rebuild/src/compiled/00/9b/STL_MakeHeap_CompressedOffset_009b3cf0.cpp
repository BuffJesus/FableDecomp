// Native STLport MakeHeap for compressed-definition index/offset pairs at 0x009B3CF0.
#include "fable_vector_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I first,I last,C comp,T*,D*) {
 if(last-first<2)return;
 D len=last-first;D parent=(len-2)/2;
 for(;;){__adjust_heap(first,parent,len,T(*(first+parent)),comp);if(parent==0)return;--parent;}
}
}
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Comp;
template void FABLE_FASTCALL _STL::__make_heap(Entry*,Entry*,Comp,Entry*,int*);
