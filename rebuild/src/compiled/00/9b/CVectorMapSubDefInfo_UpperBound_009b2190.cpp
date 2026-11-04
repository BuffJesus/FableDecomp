// CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<...> >
// ::UpperBound(const unsigned long&) @ 0x009B2190 (FableWin
// ?UpperBound@?$CVectorMap@...@@QAE...@ABK@Z; from EraseFromKey in
// CParentDefClassBase::RemoveSubDef 0x009B0170).
#include "fable_def_class_base.h"

// Binary search of the sorted entries for the first key above `key`.
template <class K, class V, class C>
_STL::pair<K, V>* CVectorMap<K, V, C>::UpperBound(const K& key)
{
    _STL::pair<K, V>* first = this->begin();
    long count = this->end() - first;
    while (count > 0)
    {
        long step = count / 2;
        _STL::pair<K, V>* middle = first + step;
        if (!(key < middle->first))
        {
            first = middle + 1;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

template _STL::pair<unsigned long, CDefClassBase::CSubDefInfo>*
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
           CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >::UpperBound(const unsigned long& key);
