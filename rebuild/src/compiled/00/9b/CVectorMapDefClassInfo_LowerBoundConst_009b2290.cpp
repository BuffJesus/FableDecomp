// CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo,
//     CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) const @ 0x009B2290
// (the const overload; caller the const CDefinitionManager::GetPDefClassInfo
// 0x009AD2E0). Same body as the non-const 0x009B2230.
#include "fable_definition_manager.h"

// Binary search of the sorted entries for the first key not below `key`.
template <class K, class V, class C>
const _STL::pair<K, V>* CVectorMap<K, V, C>::LowerBound(const K& key) const
{
    const _STL::pair<K, V>* first = this->begin();
    long count = this->end() - first;
    while (count > 0)
    {
        long step = count / 2;
        const _STL::pair<K, V>* middle = first + step;
        if (middle->first < key)
        {
            first = middle + 1;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template const _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo>*
CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo,
           CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >::LowerBound(const unsigned long& key) const;