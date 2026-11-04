#include "fable_string.h"

inline CCharString::CCharString() : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;
}

inline CCharString::CCharString(const CCharString& other) : storage_(0)
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

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

CCharString FABLE_FASTCALL GFIntToCharString(long value)
{
    CCharString result;
    result.Sprintf("%d", value);
    return result;
}
