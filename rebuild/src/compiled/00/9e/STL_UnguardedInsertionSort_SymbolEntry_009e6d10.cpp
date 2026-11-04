#include "fable_symbol_map.h"
#include "fable_stlport_linear_insert.h"
// Keep the called insertion helper out of this provider; its existing header
// exposes the body and would emit a duplicate COMDAT definition here.
namespace _STL {
template<class I,class T,class C>
void FABLE_FASTCALL __unguarded_insertion_sort_aux(I first,I last,T*,C comp) {
 for(I i=first;i!=last;++i)__unguarded_linear_insert(i,T(*i),comp);
}
}
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__unguarded_insertion_sort_aux(SymbolEntry* first, SymbolEntry* last, SymbolEntry*, CCompareString<CSymbolMap::CSymbolInfo> compare);
