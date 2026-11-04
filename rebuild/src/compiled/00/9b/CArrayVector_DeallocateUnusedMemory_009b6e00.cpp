#include "fable_definition_manager.h"
#include "fable_stlport_vector_impl.h"
#include <new>

// Retail instantiations: unsigned long 0x009B6E00 and instantiated-def
// pointers 0x009B6EB0. Each shrinks only when capacity exceeds size.

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

template void CArrayVector<unsigned long>::DeallocateUnusedMemory();
template void CArrayVector<CDefinitionManager::CInstantiatedDefInfo*>::DeallocateUnusedMemory();
