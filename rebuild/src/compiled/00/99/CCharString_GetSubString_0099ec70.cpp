#include "fable_string.h"

__forceinline CCharString::CCharString() : storage_(0)
{
    fable_i32 count = g_CCharStringInstanceCount_013BD800;
    ++count;
    g_CCharStringInstanceCount_013BD800 = count;
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

CCharString CCharString::GetSubString(unsigned long start, unsigned long end) const
{
    CCharString result;
    CCharStringData* newStorage = 0;
    CCharStringData* storage = storage_;
    const char* base;
    if (!storage)
        base = g_FableEmptyNarrowString_0129AAF4;
    else
        base = storage->Data.PData;
    const char* sub = base + start;
    if (sub != 0 && *sub != 0)
    {
        newStorage = result.AllocStringData(sub, static_cast<long>(end - start));
        result.storage_ = newStorage;
    }
    return result;
}
