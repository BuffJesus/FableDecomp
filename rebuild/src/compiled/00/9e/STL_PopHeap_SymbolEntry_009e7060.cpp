#include "fable_symbol_map.h"
#include "fable_stlport_heap.h"
namespace _STL {
template<class I,class T,class C,class D> void FABLE_FASTCALL __pop_heap(I first,I last,I result,T value,C comp,D*) {
 *result=*first;
 __adjust_heap(first,D(0),D(last-first),value,comp);
}
}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
typedef CCompareString<CSymbolMap::CSymbolInfo> Comp;
template void FABLE_FASTCALL _STL::__pop_heap(Entry*,Entry*,Entry*,Entry,Comp,int*);
