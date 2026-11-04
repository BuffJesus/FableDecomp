#include "fable_symbol_map.h"
#include "fable_stlport_unguarded_linear_insert.h"
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__unguarded_linear_insert(SymbolEntry* last, SymbolEntry value, CCompareString<CSymbolMap::CSymbolInfo> compare);
