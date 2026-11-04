// _STL::vector<pair<CCharString, CSymbolMap::CSymbolInfo> >::reserve(size_t)
// @ 0x009E72C0 (retail SymbolMap.cpp). The body is STLport's, in
// fable_stlport_vector_impl.h: grow to exactly n elements when the capacity
// is smaller, copying the entries over and releasing the old block.
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"

#include <new>

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void _STL::vector<SymbolEntry>::reserve(size_t __n);