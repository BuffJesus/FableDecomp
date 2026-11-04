#include "fable_memory_file.h"
#include <new>
#include "fable_stlport_vector_impl.h"
#include "fable_stlport_back_insert.h"
void CMemoryFile::Copy(const CMemoryFile& other)
{
    AccessType = other.AccessType;
    Position = other.Position;
    Length = other.Length;
    Opened = other.Opened;
    Pathname = other.Pathname;
    _STL::copy(other.DataBuffer.begin(), other.DataBuffer.end(), _STL::back_inserter(DataBuffer));
}
