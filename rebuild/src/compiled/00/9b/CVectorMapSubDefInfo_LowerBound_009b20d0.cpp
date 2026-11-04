// CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<...> >
// ::LowerBound(const unsigned long&) @ 0x009B20D0 (caller
// CParentDefClassBase::GetPSubDefInfo 0x009AD510). The body is the same as the
// CDefClassInfo map's (0x009B2230).
#include "fable_def_class_base.h"

#ifndef FABLE_SUBDEF_LOWER_BOUND_BODY
#define FABLE_SUBDEF_LOWER_BOUND_BODY
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
#endif

template _STL::pair<unsigned long, CDefClassBase::CSubDefInfo>*
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
           CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >::LowerBound(const unsigned long& key);
