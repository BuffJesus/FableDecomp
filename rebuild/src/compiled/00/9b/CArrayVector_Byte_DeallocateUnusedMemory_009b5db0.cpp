#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"
#include <new>

template<class T>
void CArrayVector<T>::DeallocateUnusedMemory()
{
    if (this->capacity() > this->size())
    {
        CArrayVector<T> replacement;
        replacement.reserve(this->size());
        replacement.insert(replacement.begin(), this->begin(), this->end());
        this->DeallocateMemory();
        this->swap(replacement);
    }
}
template void CArrayVector<unsigned char>::DeallocateUnusedMemory();
