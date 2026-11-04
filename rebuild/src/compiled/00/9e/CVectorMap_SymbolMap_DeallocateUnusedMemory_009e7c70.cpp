// CVectorMap<CCharString, CSymbolMap::CSymbolInfo, CCompareString<CSymbolInfo> >::
// DeallocateUnusedMemory @ 0x009E7C70 (Ego_r's CArray<pair<...> > member;
// retail SymbolMap.cpp, tail-called by CSymbolMap::UpdateDirty 0x009E5C30).
#include "fable_symbol_map.h"
#include "fable_stlport_vector_impl.h"

#include <new>

// Shrink the storage to the size: copy into an exactly reserved array, release
// this one and take the copy's storage.
template <class K, class V, class C>
void CVectorMap<K, V, C>::DeallocateUnusedMemory()
{
    if (this->capacity() > this->size())
    {
        _STL::vector<_STL::pair<K, V> > __tmp;
        __tmp.reserve(this->size());
        __tmp.insert(__tmp.begin(), this->begin(), this->end());
        CArrayVector<_STL::pair<K,V> >::DeallocateMemory();
        this->swap(__tmp);
    }
}

template void CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
                         CCompareString<CSymbolMap::CSymbolInfo> >::DeallocateUnusedMemory();