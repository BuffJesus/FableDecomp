#include "fable_vector_map.h"
#include "fable_stlport_vector_impl.h"

namespace _STL {
// STLport's class-element erase retains its typed copy loop even for clear().
template<> inline pair<unsigned long,unsigned long>*
vector<pair<unsigned long,unsigned long> >::erase(
    pair<unsigned long,unsigned long>* first,
    pair<unsigned long,unsigned long>* last)
{
    pair<unsigned long,unsigned long>* result =
        __copy(last, _M_finish, first, random_access_iterator_tag(), (ptrdiff_t*)0);
    _Destroy(result, _M_finish);
    _M_finish=result;
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
template void CArrayVector<unsigned char>::DeallocateMemory();
template void CArrayVector<_STL::pair<unsigned long,unsigned long> >::DeallocateMemory();
