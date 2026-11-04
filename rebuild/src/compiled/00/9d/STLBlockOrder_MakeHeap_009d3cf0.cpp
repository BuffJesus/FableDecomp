// Native STLport MakeHeap for CBankFile::InitBlockOrder entry indices at 0x009D3CF0.
#include "fable_game_text_bank.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C,class T,class D> void FABLE_FASTCALL __make_heap(I first,I last,C comp,T*,D*) {
 if(last-first<2)return;
 D len=last-first;D parent=(len-2)/2;
 for(;;){__adjust_heap(first,parent,len,T(*(first+parent)),comp);if(parent==0)return;--parent;}
}
}
typedef CBankFile::CompareDataOffset Comp;
template void FABLE_FASTCALL _STL::__make_heap(unsigned long*,unsigned long*,Comp,unsigned long*,int*);
