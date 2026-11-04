#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_algobase.h"

// Retail instantiations: 0x009B4A90, 0x009B4B80, and 0x009B5F30.
// The child pair uses STLport's typed copy even when erase's range is empty.

namespace _STL {
template<> inline pair<unsigned long, CDefClassBase::CSubDefInfo>*
vector<pair<unsigned long, CDefClassBase::CSubDefInfo> >::erase(
    pair<unsigned long, CDefClassBase::CSubDefInfo>* first,
    pair<unsigned long, CDefClassBase::CSubDefInfo>* last)
{
    pair<unsigned long, CDefClassBase::CSubDefInfo>* result =
        __copy(last, _M_finish, first, random_access_iterator_tag(), (ptrdiff_t*)0);
    _Destroy(result, _M_finish);
    _M_finish = result;
    return first;
}
}

template<class T>
void CArrayVector<T>::DeallocateMemory()
{
    if (this->capacity() > 0)
    {
        this->clear();
        _STL::vector<T> empty;
        this->swap(empty);
    }
}

template void CArrayVector<unsigned long>::DeallocateMemory();
template void CArrayVector<CDefinitionManager::CInstantiatedDefInfo*>::DeallocateMemory();
template void CArrayVector<_STL::pair<unsigned long, CDefClassBase::CSubDefInfo> >
    ::DeallocateMemory();
