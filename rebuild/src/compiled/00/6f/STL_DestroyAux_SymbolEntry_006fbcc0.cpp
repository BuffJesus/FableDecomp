// _STL::__destroy_aux<pair<CCharString, CSymbolMap::CSymbolInfo>*>(first, last,
// __false_type const&) @ 0x006FBCC0 (a byte-identical instantiation shared
// with other pair<CCharString, ...> vectors). The body (destroy [first, last)
// in place: entry name, group name, then the key) is STLport's, in
// fable_stlport_uninitialized.h.
#include "fable_symbol_map.h"
#include "fable_stlport_uninitialized.h"

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__destroy_aux(SymbolEntry* __first, SymbolEntry* __last,
                                                 const _STL::__false_type&);
