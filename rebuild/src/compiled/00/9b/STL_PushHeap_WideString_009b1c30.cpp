// STLport wide-string heap insertion called by 0x009B2EA0.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class D,class T,class C>
void FABLE_FASTCALL __push_heap(I first,D hole,D top,T value,C comp)
{
    D parent=(hole-1)/2;
    while(hole>top && comp(*(first+parent),value))
    {
        *(first+hole)=*(first+parent);
        hole=parent;
        parent=(hole-1)/2;
    }
    *(first+hole)=value;
}
}
template void FABLE_FASTCALL _STL::__push_heap(CWideString*,int,int,CWideString,_STL::less<CWideString>);
