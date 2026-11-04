// CWideString::operator+=(const CWideString&) @ 0x0099B8D0 (FableWin
// ??YCWideString@@QAEABV0@ABV0@@Z). Appends the other text to this string's
// own copy of its data (STLport basic_string<wchar_t>::append 0x0099C7E0); an
// empty string shares the other's data instead. The copy assignment
// (0x0099B7D0) is expanded here, so it is defined inline in this unit as the
// operator+ units do with UnassignString.
#include "fable_string.h"

inline CWideString& CWideString::operator=(const CWideString& other)
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

const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        storage_->Data.append(other.storage_ == 0 ? L"" : other.storage_->text);
    }
    else
    {
        *this = other;
    }
    return *this;
}
