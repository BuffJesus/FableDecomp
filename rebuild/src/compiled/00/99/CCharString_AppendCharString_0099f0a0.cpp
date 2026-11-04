// CCharString::operator+=(CCharString const&) @ 0x0099F0A0 (FableWin
// ??YCCharString@@QAEABV0@ABV0@@Z).
#include "fable_string.h"

// Append the other string's text to an own copy of this one; an empty string
// just shares the other's data.
const CCharString& CCharString::operator+=(const CCharString& other)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        const char* text = other.GetConstCharPtr();
        CCharStringData* data = storage_;
        long length = 0;
        if (text[0] != '\0')
        {
            do
                ++length;
            while (text[length] != '\0');
        }
        data->Data.Append(text, length);
        return *this;
    }
    if (&other != this && other.storage_ != 0)
    {
        storage_ = other.storage_;
        ++storage_->owners;
    }
    return *this;
}
