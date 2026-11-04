// _STL::__adjust_heap<pair<unsigned long, CDefClassBase::CSubDefInfo>*, int, ...> @ 0x009B2F30, from the heap
// builders and __partial_sort's inline __pop_heap.
#include "fable_def_class_base.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C comp) {
 const D top=hole;D child=2*hole+2;
 while(child<len){if(comp(*(first+child),*(first+child-1)))--child;*(first+hole)=*(first+child);hole=child;child=2*(child+1);}
 if(child==len){*(first+hole)=*(first+child-1);hole=child-1;}
 __push_heap(first,hole,top,value,comp);
}
}
typedef _STL::pair<unsigned long,CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Comp;
template void FABLE_FASTCALL _STL::__adjust_heap(Entry*,int,int,Entry,Comp);
