// _STL::__insertion_sort<pair<CCharString, CSymbolMap::CSymbolInfo>*,
// CCompareString<CSymbolMap::CSymbolInfo> >(first, last, comp) @ 0x009E7590
// (retail SymbolMap.cpp, from sort 0x009E7E40). The body is STLport's, in
// fable_stlport_insertion_sort.h.
#include "fable_symbol_map.h"
#include "fable_stlport_insertion_sort.h"

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__insertion_sort(SymbolEntry* __first, SymbolEntry* __last,
                                                    CCompareString<CSymbolMap::CSymbolInfo> __comp);
