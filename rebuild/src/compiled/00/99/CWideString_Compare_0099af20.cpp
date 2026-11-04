// lib_wide_string.cpp, retail 0x0099AF20-0x0099B260: comparisons, the
// substring test, file output and CRC (Ego_r names). An empty string reads as
// L"" (0x0129A8E0).
#include <wchar.h>
#include "fable_string.h"
#include "fable_filesystem.h"

inline const wchar_t* CWideString::GetAsCharArray() const
{
    if (storage_ == 0)
        return g_FableEmptyWideString_0129A8E0;
    return storage_->text;
}

inline long CWideString::GetLength() const
{
    if (storage_ == 0)
        return 0;
    return static_cast<long>(storage_->Data.size());
}

// Retail 0099AF20.
bool CWideString::Contains(const CWideString& other) const
{
    return wcsstr(GetAsCharArray(), other.GetAsCharArray()) != 0;
}

// Retail 0099AFB0.
bool CWideString::operator<=(const CWideString& other) const
{
    return wcscmp(GetAsCharArray(), other.GetAsCharArray()) <= 0;
}

// Retail 0099AFF0.
bool CWideString::operator>(const CWideString& other) const
{
    return wcscmp(GetAsCharArray(), other.GetAsCharArray()) > 0;
}

// Retail 0099B030.
bool CWideString::operator>=(const CWideString& other) const
{
    return wcscmp(GetAsCharArray(), other.GetAsCharArray()) >= 0;
}

// Retail 0099B0C0: the characters without a terminator; nothing when empty.
void CWideString::SaveToFile(CAFile& file)
{
    if (storage_ != 0)
    {
        const wchar_t* text = storage_->text;
        file.Write(text, wcslen(text) * sizeof(wchar_t), 0);
    }
}

// Retail 0099B1D0: a one-character comparison; an empty string differs.
bool CWideString::operator!=(wchar_t character) const
{
    bool equal;
    if (storage_ != 0)
    {
        wchar_t text[2] = {character, 0};
        equal = wcscmp(storage_->text, text) == 0;
    }
    else
        equal = false;
    return !equal;
}

// Retail 0099B260.
unsigned long CWideString::GetCRC() const
{
    return FableCRC_Calc_004014A0(0, GetAsCharArray(), GetLength() * sizeof(wchar_t));
}
