// Ego_r.pdb CWideString::GetByteLength; retail 0x0099B240.
#include "fable_string.h"

long CWideString::GetByteLength() const
{
    return GetLength() * sizeof(wchar_t);
}

inline long CWideString::GetLength() const
{
    if (storage_ == 0)
        return 0;
    return storage_->Data._M_finish - storage_->Data._M_start;
}
