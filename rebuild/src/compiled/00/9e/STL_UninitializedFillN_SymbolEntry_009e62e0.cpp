// _STL::__uninitialized_fill_n<pair<CCharString, CSymbolMap::CSymbolInfo>*,
// unsigned int, pair<...> >(first, n, x, __false_type const&) @ 0x009E62E0
// (SymbolMap.cpp). The body (copy-construct n copies of x into raw storage
// at first; returns the end) is STLport's, in fable_stlport_uninitialized.h.
#include "fable_symbol_map.h"
#include "fable_stlport_uninitialized.h"

#include <new>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template SymbolEntry* FABLE_FASTCALL _STL::__uninitialized_fill_n(SymbolEntry* __first, size_t __n,
                                                                  const SymbolEntry& __x,
                                                                  const _STL::__false_type&);
