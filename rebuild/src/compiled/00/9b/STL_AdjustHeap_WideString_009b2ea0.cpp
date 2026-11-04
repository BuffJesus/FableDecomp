// STLport wide-string heap adjustment called by make_heap and sort_heap.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C>
void FABLE_FASTCALL __adjust_heap(I first,D hole,D len,T value,C comp)
{
    const D top=hole;
    D child=2*hole+2;
    while(child<len)
    {
        if(comp(*(first+child),*(first+child-1)))--child;
        *(first+hole)=*(first+child);
        hole=child;
        child=2*(child+1);
    }
    if(child==len)
    {
        *(first+hole)=*(first+child-1);
        hole=child-1;
    }
    __push_heap(first,hole,top,value,comp);
}
}
template void FABLE_FASTCALL _STL::__adjust_heap(CWideString*,int,int,CWideString,_STL::less<CWideString>);
