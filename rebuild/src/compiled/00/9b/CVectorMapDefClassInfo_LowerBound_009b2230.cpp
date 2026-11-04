// CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo,
//     CKeyPairCompareLess<...> >::LowerBound(const unsigned long&) @ 0x009B2230
// (FableWin ?LowerBound@?$CVectorMap@KVCDefClassInfo@CDefinitionManager@@...@@QAE...@ABK@Z;
// caller CDefinitionManager::GetPDefClassInfo 0x009AD580). 0x009B2290 is the
// same body for the const overload.
#include "fable_definition_manager.h"

// Binary search of the sorted entries for the first key not below `key`.
template <class K, class V, class C>
_STL::pair<K, V>* CVectorMap<K, V, C>::LowerBound(const K& key)
{
    _STL::pair<K, V>* first = this->begin();
    long count = this->end() - first;
    while (count > 0)
    {
        long step = count / 2;
        _STL::pair<K, V>* middle = first + step;
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

template _STL::pair<unsigned long, CDefinitionManager::CDefClassInfo>*
CVectorMap<unsigned long, CDefinitionManager::CDefClassInfo,
           CKeyPairCompareLess<unsigned long, CDefinitionManager::CDefClassInfo> >::LowerBound(const unsigned long& key);