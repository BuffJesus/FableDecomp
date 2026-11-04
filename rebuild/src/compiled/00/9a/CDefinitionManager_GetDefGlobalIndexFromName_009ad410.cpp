#include "fable_definition_manager.h"
typedef CVectorMap<unsigned long,long,CKeyPairCompareLess<unsigned long,long> > NameMap;
static __forceinline const _STL::pair<unsigned long,long>* FindName(const NameMap& map,unsigned long key,const _STL::pair<unsigned long,long>* last)
{
    const unsigned long searchKey=key;
    const _STL::pair<unsigned long,long>* it=map.LowerBound(searchKey);
    if(it==last || key<it->first)return last;
    return it;
}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    if(name.IsEmpty()) return 0;
    long index;
    const _STL::pair<unsigned long,long>* last=NameCRCToIndexMap.end();
    if(NameCRCToIndexMap.begin()!=last)
    {
        const _STL::pair<unsigned long,long>* it=FindName(NameCRCToIndexMap,name.GetCRC(),last);
        if(it!=NameCRCToIndexMap.end()) return it->second;
    }
    else
    {
        if(PDefinitionSymbolMap->FindSymbol(name,index)) return index;
    }
    return 0;
}
