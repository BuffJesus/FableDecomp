// _STL::__make_heap<pair<unsigned long, CDefClassBase::CSubDefInfo>*, ...> @ 0x009B3C00, from __partial_sort
// 0x009B6AD0.
#include "fable_def_class_base.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I first,I last,C comp,T*,D*) {
 if(last-first<2)return;
 D len=last-first;D parent=(len-2)/2;
 for(;;){__adjust_heap(first,parent,len,T(*(first+parent)),comp);if(parent==0)return;--parent;}
}
}
typedef _STL::pair<unsigned long,CDefClassBase::CSubDefInfo> Entry;
typedef CKeyPairCompareLess<unsigned long,CDefClassBase::CSubDefInfo> Comp;
template void FABLE_FASTCALL _STL::__make_heap(Entry*,Entry*,Comp,Entry*,int*);
