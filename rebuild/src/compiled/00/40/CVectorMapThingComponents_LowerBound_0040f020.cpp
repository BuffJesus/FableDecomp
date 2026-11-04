#include "fable_world.h"
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

template _STL::pair<ETCInterfaceType,CTCBase*>* CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key);
