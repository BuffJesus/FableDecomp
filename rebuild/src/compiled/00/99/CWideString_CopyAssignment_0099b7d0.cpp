#include "fable_string.h"

CWideString& CWideString::operator=(const CWideString& other)
{
    if (&other != this && other.storage_ != storage_)
    {
        if (storage_ != 0)
            UnassignString();
        if (other.storage_ != 0)
        {
            storage_ = other.storage_;
            ++storage_->owners;
        }
    }
    return *this;
}
