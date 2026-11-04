// _STL::__uninitialized_copy<pair<CCharString, CSymbolMap::CSymbolInfo>*, ...>
// (first, last, result, __false_type const&) @ 0x009E6270 (SymbolMap.cpp).
// The body (copy-construct [first, last) into raw storage at result; returns
// the end) is STLport's, in fable_stlport_uninitialized.h.
#include "fable_symbol_map.h"
#include "fable_stlport_uninitialized.h"

#include <new>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template SymbolEntry* FABLE_FASTCALL _STL::__uninitialized_copy(SymbolEntry* __first, SymbolEntry* __last,
                                                                SymbolEntry* __result, const _STL::__false_type&);
