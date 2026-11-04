// CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<...> >
// ::LowerBound(const unsigned long&) const @ 0x009B2130 (caller
// CParentDefClassBase::GetPSubDefInfo const 0x009AD4A0).
#include "fable_def_class_base.h"

#ifndef FABLE_SUBDEF_LOWER_BOUND_CONST_BODY
#define FABLE_SUBDEF_LOWER_BOUND_CONST_BODY
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
#endif

template const _STL::pair<unsigned long, CDefClassBase::CSubDefInfo>*
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
           CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >::LowerBound(const unsigned long& key) const;
