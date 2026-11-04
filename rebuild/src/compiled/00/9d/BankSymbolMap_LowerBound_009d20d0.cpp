#include "fable_vector_map.h"

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

template const _STL::pair<unsigned long, unsigned long>*
CVectorMap<unsigned long, unsigned long,
           CKeyPairCompareLess<unsigned long, unsigned long> >::LowerBound(const unsigned long& key) const;