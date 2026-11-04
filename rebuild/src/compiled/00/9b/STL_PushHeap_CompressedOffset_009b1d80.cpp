// Native STLport PushHeap for compressed-definition index/offset pairs at 0x009B1D80.
#include "fable_vector_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C comp) {
 D parent=(hole-1)/2;
 while(hole>top&&comp(*(first+parent),value)){*(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;}
 *(first+hole)=value;
}
}
typedef _STL::pair<unsigned long, unsigned long> Entry;
typedef CKeyPairCompareLess<unsigned long, unsigned long> Comp;
template void FABLE_FASTCALL _STL::__push_heap(Entry*,int,int,Entry,Comp);
