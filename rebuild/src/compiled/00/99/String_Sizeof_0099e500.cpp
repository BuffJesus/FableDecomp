#include "fable_string.h"
unsigned long CCharString::Sizeof() const
{
    unsigned long size = sizeof(*this);
    if (storage_) size = sizeof(*this) + sizeof(CCharStringData) + storage_->Data.DataLength;
    return size;
}
unsigned long CWideString::Sizeof() const
{
    unsigned long size = sizeof(*this);
    if (storage_) size = sizeof(*this) + storage_->Data.size() * sizeof(wchar_t);
    return size;
}
