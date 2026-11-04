#include "fable_vector_map.h"
#include "fable_stlport_vector_impl.h"
#include <new>

template<class K,class V,class C>
void CVectorMap<K,V,C>::DeallocateUnusedMemory()
{
    if (this->capacity() > this->size())
    {
        _STL::vector<_STL::pair<K,V> > replacement;
        replacement.reserve(this->size());
        replacement.insert(replacement.begin(), this->begin(), this->end());
        CArrayVector<_STL::pair<K,V> >::DeallocateMemory();
        this->swap(replacement);
    }
}
template void CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> >::DeallocateUnusedMemory();
