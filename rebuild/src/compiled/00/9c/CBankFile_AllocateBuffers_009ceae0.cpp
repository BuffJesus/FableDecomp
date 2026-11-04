// Retail009CEAE0: rebuild runtime storage, allocate optional arrays by open flags,
// and clear symbol/filename indices. Runtime value initialization leaves padding alone.
#include "fable_game_text_bank.h"
#include "fable_stlport_char_string_vector.h"
#include "fable_stlport_algobase.h"
template<> inline CBankFile::CRuntimeData* _STL::vector<CBankFile::CRuntimeData>::erase(CBankFile::CRuntimeData* first, CBankFile::CRuntimeData* last)
{
    CBankFile::CRuntimeData* out = _STL::copy(last, this->_M_finish, first);
    this->_M_finish = out;
    return first;
}
namespace _STL { template<> void vector<CCharString>::resize(size_t); }
template<class T> void CArrayVector<T>::DeallocateMemory()
{
    if (this->capacity() > 0) {
        this->clear();
        _STL::vector<T> empty;
        this->swap(empty);
    }
}
void CBankFile::AllocateBuffers(unsigned long size)
{
    Size = size;
    RuntimeData.DeallocateMemory();
    CRuntimeData empty;
    empty.DataOffset=0;empty.DataSize=0;empty.DataType=0;empty.Valid=false;
    RuntimeData.resize(size, empty);
    Symbols.DeallocateMemory();
    if (OpenFlags & 8) Symbols.resize(size);
    Checksums.DeallocateMemory();
    if (OpenFlags & 16) Checksums.resize(size);
    UpdateData.DeallocateMemory();
    if (OpenFlags & 32) UpdateData.resize(size);
    SymbolIndices.clear();
    FilenameIndices.clear();
}
