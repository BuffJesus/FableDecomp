#include "fable_string.h"

CCharString& CCharString::operator=(const CCharString& other)
{
    if (&other != this)
    {
        if (storage_ != 0)
            UnassignString();
        CCharStringData* otherStorage =
            *reinterpret_cast<CCharStringData* const*>(&other);
        if (otherStorage != 0)
        {
            storage_ = otherStorage;
            ++storage_->owners;
        }
    }
    return *this;
}
