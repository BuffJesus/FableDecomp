// CCharString::CCharString(const CCharString&) @ 0x0099EC30
// (FableWin ??0CCharString@@QAE@ABV0@@Z).
//
// Counts the new instance and shares the other string's data (one more
// owner); a self-copy leaves the string empty.

#include "fable_string.h"

CCharString::CCharString(const CCharString& other)
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;

    if (&other != this)
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
