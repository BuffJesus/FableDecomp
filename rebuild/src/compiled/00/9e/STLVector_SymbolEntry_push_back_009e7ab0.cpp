// _STL::vector<_STL::pair<CCharString, CSymbolMap::CSymbolInfo> >::push_back
// (pair const&) @ 0x009E7AB0 (instantiated in retail's SymbolMap.cpp). The
// body (copy the entry into the spare slot, or grow the storage by one) is
// STLport's, in fable_stlport_vector_impl.h.
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"

#include <new>

template void _STL::vector<_STL::pair<CCharString, CSymbolMap::CSymbolInfo> >::push_back(
    const _STL::pair<CCharString, CSymbolMap::CSymbolInfo>& __x);
