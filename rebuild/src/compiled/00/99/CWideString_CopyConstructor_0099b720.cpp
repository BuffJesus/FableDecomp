#include "fable_string.h"

CWideString::CWideString(const CWideString& other)
    : storage_(0)
{
    ++g_CWideStringInstanceCount_013BCA20;

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
}
