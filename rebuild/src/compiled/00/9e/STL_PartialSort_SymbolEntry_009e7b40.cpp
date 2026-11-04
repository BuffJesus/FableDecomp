#include "fable_symbol_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class T,class C> void FABLE_FASTCALL __partial_sort(I first,I middle,I last,T*,C comp) {
 __make_heap(first,middle,comp,(T*)0,(int*)0);
 for(I i=middle;i<last;++i)if(comp(*i,*first))__pop_heap(first,middle,i,T(*i),comp,(int*)0);
 sort_heap(first,middle,comp);
}
}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Comp;
template void FABLE_FASTCALL _STL::__partial_sort(Entry*,Entry*,Entry*,Entry*,Comp);
