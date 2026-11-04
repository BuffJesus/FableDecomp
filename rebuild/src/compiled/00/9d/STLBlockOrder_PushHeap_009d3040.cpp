// Native STLport PushHeap for CBankFile::InitBlockOrder entry indices at 0x009D3040.
#include "fable_game_text_bank.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C comp) {
 D parent=(hole-1)/2;
 while(hole>top&&comp(*(first+parent),value)){*(first+hole)=*(first+parent);hole=parent;parent=(hole-1)/2;}
 *(first+hole)=value;
}
}
typedef CBankFile::CompareDataOffset Comp;
template void FABLE_FASTCALL _STL::__push_heap(unsigned long*,int,int,unsigned long,Comp);
