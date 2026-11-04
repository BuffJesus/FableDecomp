// _STL::vector<_STL::pair<CCharString, CSymbolMap::CSymbolInfo> >::_M_insert_overflow
// (pair*, pair const&, __false_type const&, unsigned int, bool) @ 0x009E77E0
// (instantiated in retail's SymbolMap.cpp). The body is STLport's, in
// fable_stlport_vector_impl.h; the helper bodies it calls must be visible (see
// fable_stlport_uninitialized.h) for VC7.1 to overlay the tag temporaries.
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"

#include <new>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void _STL::vector<SymbolEntry>::_M_insert_overflow(SymbolEntry* __position, const SymbolEntry& __x,
                                                            const _STL::__false_type&, size_t __fill_len,
                                                            bool __atend);
