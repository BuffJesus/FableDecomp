// _STL::__introsort_loop<pair<CCharString, CSymbolMap::CSymbolInfo>*, pair<...>, int,
// CCompareString<CSymbolMap::CSymbolInfo> >(first, last, _Tp*, depth_limit, comp)
// @ 0x009E7D30 (retail SymbolMap.cpp, from sort 0x009E7E40). The body is
// STLport's, in fable_stlport_introsort.h.
#include "fable_symbol_map.h"
#include "fable_stlport_introsort.h"

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__introsort_loop(SymbolEntry* __first, SymbolEntry* __last, SymbolEntry*,
                                                    int __depth_limit,
                                                    CCompareString<CSymbolMap::CSymbolInfo> __comp);
