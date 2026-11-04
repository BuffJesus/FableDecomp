#include "fable_symbol_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class C> void FABLE_FASTCALL sort_heap(I first,I last,C comp) {
 typedef typename iterator_traits<I>::value_type T;
 while(last-first>1){__pop_heap(first,last-1,last-1,T(*(last-1)),comp,(int*)0);--last;}
}
}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Comp;
template void FABLE_FASTCALL _STL::sort_heap(Entry*,Entry*,Comp);
