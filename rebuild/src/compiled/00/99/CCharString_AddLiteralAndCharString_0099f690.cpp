// operator+(char const*, CCharString const&) @ 0x0099F690 (FableWin
// ??H@YI?AVCCharString@@PBDABV0@@Z; AddDefClass 0x009B0AC0 builds
// "NULLDEF_" + class name with it).
#include "fable_string.h"

// Retail compiles these members with operator+ in one unit and expands them
// here; they are inline so the unit links beside their own landings
// (0x0099EBF0, 0x0099EA60, 0x0099EC30, 0x0099EAE0), whose code is unchanged.
inline CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    CCharStringData* data;
    if (length == -1)
        data = new CCharStringData(text);
    else
        data = new CCharStringData(text, length);
    return data;
}

inline CCharString::CCharString(const char* text, long length)
    : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;

    if (storage_ != 0)
        UnassignString();

    if (text != 0 && text[0] != '\0')
        storage_ = AllocStringData(text, length);
}

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

// The literal, then the string appended.
CCharString FABLE_FASTCALL operator+(const char* left, const CCharString& right)
{
    CCharString result(left);
    result += right;
    return result;
}