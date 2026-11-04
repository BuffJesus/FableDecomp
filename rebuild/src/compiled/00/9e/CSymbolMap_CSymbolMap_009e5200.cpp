// CSymbolMap::CSymbolMap() @ 0x009E5200.
#include "fable_symbol_map.h"

// An empty symbol table: no symbols, no names used, duplicates not checked.
CSymbolMap::CSymbolMap()
    : CheckForDuplicateSymbols(false)
{
}
