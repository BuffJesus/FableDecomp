#include "fable_string.h"

// lib_string.cpp unsigned formatters (Ego_r parameter n), retail 0099F8B0 and
// 0099F930. The CCharString lifetime members are expanded in this unit, as in
// GFIntToCharString 0x0099F830.

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

CCharString FABLE_FASTCALL GFUIntToCharString(unsigned long n)
{
    CCharString result;
    result.Sprintf("%u", n);
    return result;
}

CCharString FABLE_FASTCALL GFUHUGEToCharString(unsigned __int64 n)
{
    CCharString result;
    result.Sprintf("%I64u", n);
    return result;
}
