#include "fable_symbol_map.h"
#include "fable_stlport_linear_insert.h"
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> SymbolEntry;
template void FABLE_FASTCALL _STL::__linear_insert(SymbolEntry* first, SymbolEntry* last, SymbolEntry value, CCompareString<CSymbolMap::CSymbolInfo> compare);
