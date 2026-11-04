// _STL::sort<pair<CCharString, CSymbolMap::CSymbolInfo>*, CCompareString<CSymbolMap::CSymbolInfo> >
// (first, last, comp) @ 0x009E7E40 (retail SymbolMap.cpp, from CVectorMap::Sort).
// The body is STLport's, in fable_stlport_algo_impl.h; here the final
// insertion pass is expanded inline.
#include "fable_symbol_map.h"
#include "fable_stlport_algo_impl.h"

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::sort(SymbolEntry* __first, SymbolEntry* __last,
                                        CCompareString<CSymbolMap::CSymbolInfo> __comp);
