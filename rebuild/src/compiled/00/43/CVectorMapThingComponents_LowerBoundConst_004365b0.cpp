#include "fable_world.h"
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

template const _STL::pair<ETCInterfaceType,CTCBase*>* CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const;
