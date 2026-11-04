// _STL::__push_heap<pair<unsigned long, CDefClassBase::CSubDefInfo>*, int, ...> @ 0x009B1CA0, from __adjust_heap
// 0x009B2F30.
#include "fable_def_class_base.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C comp) {
 D parent=(hole-1)/2;
 while(hole>top&&comp(*(first+parent),value)){*(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;}
 *(first+hole)=value;
}
}
typedef _STL::pair<unsigned long,CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Comp;
template void FABLE_FASTCALL _STL::__push_heap(Entry*,int,int,Entry,Comp);
