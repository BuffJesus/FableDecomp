// operator+(CCharString const&, char const*) @ 0x0099F600.
#include "fable_string.h"

// Retail expands these members in this operator's translation unit.
inline CCharString::CCharString(const CCharString& other)
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

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

CCharString FABLE_FASTCALL operator+(const CCharString& left,
                                    const char* right)
{
    CCharString result(left);
    result += right;
    return result;
}
