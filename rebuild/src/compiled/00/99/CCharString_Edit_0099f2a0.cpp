#include <string.h>
#include "fable_string.h"
#include "fable_filesystem.h"

// lib_string.cpp in-place edits (Ego_r CCharString::Truncate/Erase): copy
// shared data first, then edit the CBasicString. Retail 0099F2A0, 0099F400.
// Insert 0099F3D0 is not reproduced yet (retail packs its two-byte buffer
// into the char parameter's slot).

// Retail 0099F2A0: truncating to nothing releases to the empty string.
void CCharString::Truncate(unsigned long length)
{
    if ((unsigned long)GetLength() > length)
    {
        if (length == 0)
        {
            *this = "";
            return;
        }
        MakeOwnCopyOfData();
        storage_->Data.Truncate(length);
    }
}

// Retail 0099F400: close the gap, then shorten.
void CCharString::Erase(long start, long count)
{
    if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        CBasicString<char>& data = storage_->Data;
        long end = start + count;
        memmove(data.PData + start, data.PData + end, data.StringLength - end);
        data.Truncate(data.StringLength - count);
    }
}

// Retail 0099E5F0: the text without its terminator.
void CCharString::SaveToFile(CAFile& file) const
{
    if (storage_ != 0)
        file.Write(storage_->text, strlen(storage_->text), 0);
}
