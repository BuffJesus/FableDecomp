// Native STLport AdjustHeap for compressed-definition index/offset pairs at 0x009B3060.
#include "fable_vector_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C comp) {
 const D top=hole;D child=2*hole+2;
 while(child<len){if(comp(*(first+child),*(first+child-1)))--child;*(first+hole)=*(first+child);hole=child;child=2*(child+1);}
 if(child==len){*(first+hole)=*(first+child-1);hole=child-1;}
 __push_heap(first,hole,top,value,comp);
}
}
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Comp;
template void FABLE_FASTCALL _STL::__adjust_heap(Entry*,int,int,Entry,Comp);
