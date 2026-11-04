#include "fable_symbol_map.h"
bool CSymbolMap::FindSymbol(const CCharString& name, long& value) const
{
    const _STL::pair<CCharString,CSymbolInfo>* it=SLONGMap.Find(name);
    if(it!=SLONGMap.end())
    {
        value=it->second.Val;
        return true;
    }
    return false;
}
