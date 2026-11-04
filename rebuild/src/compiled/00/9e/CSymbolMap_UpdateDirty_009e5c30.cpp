// CSymbolMap::UpdateDirty() const @ 0x009E5C30 (retail SymbolMap.cpp).
#include "fable_symbol_map.h"

// Sort the symbols appended since the last update, then trim the storage.
void CSymbolMap::UpdateDirty() const
{
    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> >& map =
        const_cast<CSymbolMap*>(this)->SLONGMap;
    map.Update();
    map.DeallocateUnusedMemory();
}