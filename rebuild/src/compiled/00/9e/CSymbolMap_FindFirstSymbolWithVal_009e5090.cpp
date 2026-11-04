#include "fable_symbol_map.h"

// Retail 009E5090: linear scan in key order for the first entry with this value.
const CSymbolMap::CSymbolInfo* CSymbolMap::FindFirstSymbolWithVal(long val) const
{
    for (const _STL::pair<CCharString,CSymbolInfo>* it = SLONGMap.begin(); it != SLONGMap.end(); ++it)
        if (it->second.Val == val)
            return &it->second;
    return 0;
}

// Retail 009E50C0: as above, restricted to one symbol group.
const CSymbolMap::CSymbolInfo* CSymbolMap::FindFirstSymbolWithVal(const CCharString& groupName, long val) const
{
    for (const _STL::pair<CCharString,CSymbolInfo>* it = SLONGMap.begin(); it != SLONGMap.end(); ++it)
        if (it->second.GroupName == groupName && it->second.Val == val)
            return &it->second;
    return 0;
}
