// CSymbolMap::AddSymbolDirty(CCharString const&, long) @ 0x009E5420 and
// CSymbolMap::AddSymbolDirty(CCharString const&, CCharString const&, long)
// @ 0x009E54E0 (both in retail's SymbolMap.cpp, with CurrentFilename).
#include "fable_symbol_map.h"

// Retail 0x013CAA30 (dynamic initializer 0x01224170, atexit thunk 0x0122A870).
CCharString CSymbolMap::CurrentFilename("NULL");

// Record the name (when duplicates are being checked) and append the symbol
// unsorted; the map is sorted on the next update.
void CSymbolMap::AddSymbolDirty(const CCharString& groupName, const CCharString& name, long val)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(name);
    SLONGMap.Insert(name, CSymbolInfo(groupName, name, val));
}

// The same, in the group of the file being parsed ("NULL" outside one).
// Written out: retail's bytes are the three-argument body inlined, and VC7.1
// will not inline a call to it here.
void CSymbolMap::AddSymbolDirty(const CCharString& name, long val)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(name);
    SLONGMap.Insert(name, CSymbolInfo(CurrentFilename, name, val));
}
