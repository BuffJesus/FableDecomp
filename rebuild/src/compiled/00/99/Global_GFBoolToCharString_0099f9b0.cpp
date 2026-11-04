#include "fable_string.h"

// lib_string.cpp formatters: GFBoolToCharString 0099F9B0 (Ego_r parameter
// val) and GFByteSizeToCharString 0099FA90 (Ego_r size/precision, local str,
// which retail constructs and never uses). The CCharString lifetime members and
// AllocStringData are expanded in this unit (landed out-of-line copies
// 0x0099E4B0, 0x0099EBF0, 0x0099EAE0, 0x0099EA60).

inline CCharString::CCharString() : storage_(0)
{
    ++g_CCharStringInstanceCount_013BD800;
}

inline CCharString::~CCharString()
{
    UnassignString();
    --g_CCharStringInstanceCount_013BD800;
}

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

CCharString FABLE_FASTCALL GFBoolToCharString(bool val)
{
    if (val == true)
        return "TRUE";
    return "FALSE";
}

CCharString FABLE_FASTCALL GFByteSizeToCharString(unsigned long size, long precision)
{
    if (size >= 4096)
    {
        CCharString str;
        if (size >= 1024 * 1024)
            return GFFloatToCharString(size / (1024.0f * 1024.0f), precision) + " MB";
        return GFFloatToCharString(size / 1024.0f, precision) + " KB";
    }
    return GFIntToCharString(size) + " bytes";
}
