// Native STLport AdjustHeap for CBankFile::InitBlockOrder entry indices at 0x009D3750.
#include "fable_game_text_bank.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C> void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C comp) {
 const D top=hole;D child=2*hole+2;
 while(child<len){if(comp(*(first+child),*(first+child-1)))--child;*(first+hole)=*(first+child);hole=child;child=2*(child+1);}
 if(child==len){*(first+hole)=*(first+child-1);hole=child-1;}
 __push_heap(first,hole,top,value,comp);
}
}
typedef CBankFile::CompareDataOffset Comp;
template void FABLE_FASTCALL _STL::__adjust_heap(unsigned long*,int,int,unsigned long,Comp);
