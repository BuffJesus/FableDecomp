#include "fable_memory_file.h"
CAFile* CMemoryFile::Clone() const
{
    return new CMemoryFile(*this);
}
