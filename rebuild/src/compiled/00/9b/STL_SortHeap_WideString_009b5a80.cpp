// STLport wide-string heap sorting called by partial_sort 0x009B6A40.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class T,class C,class D>
void FABLE_FASTCALL __pop_heap(I first,I last,I result,T value,C comp,D*)
{
    *result=*first;
    __adjust_heap(first,D(0),D(last-first),value,comp);
}
template<class I,class C>
void FABLE_FASTCALL sort_heap(I first,I last,C comp)
{
    typedef typename iterator_traits<I>::value_type T;
    while(last-first>1)
    {
        __pop_heap(first,last-1,last-1,T(*(last-1)),comp,(int*)0);
        --last;
    }
}
}
template void FABLE_FASTCALL _STL::sort_heap(CWideString*,CWideString*,_STL::less<CWideString>);
