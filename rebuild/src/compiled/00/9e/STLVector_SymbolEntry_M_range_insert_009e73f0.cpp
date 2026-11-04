// _STL::vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::_M_range_insert
// (pos, first, last, forward_iterator_tag const&) @ 0x009E73F0 (retail
// SymbolMap.cpp, from CVectorMap::DeallocateUnusedMemory 0x009E7C70). The body
// is STLport's, in fable_stlport_vector_impl.h.
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"

#include <new>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void _STL::vector<SymbolEntry>::_M_range_insert(SymbolEntry* __pos, const SymbolEntry* __first,
                                                         const SymbolEntry* __last,
                                                         const _STL::forward_iterator_tag&);