// lib_wide_string.cpp, retail 0x0099BC50 and 0x0099BD00. SetLength keeps the
// text up to the new length (a new string is spaces); LoadFromFile replaces
// the string with characters read from the file. UnassignString is expanded, as in the other
// lib_wide_string units.
#include <stdlib.h>
#include "fable_filesystem.h"
#include "fable_string.h"

inline void CWideString::UnassignString()
{
    CWideStringData* shared = storage_;
    if (shared != 0)
    {
        if (--shared->owners <= 0)
        {
            CWideStringData* storage = storage_;
            if (storage != 0)
            {
                if (storage->text != 0)
                    free(storage->text);
                delete storage;
            }
        }
        storage_ = 0;
    }
}

// Retail 0099BC50.
void CWideString::SetLength(long length)
{
    if (length == 0)
        UnassignString();
    else if (storage_ != 0)
    {
        MakeOwnCopyOfData();
        storage_->Data.resize(length);
    }
    else
        storage_ = new CWideStringData(length, L' ');
}

// Retail 0099BD00. The default length (-1) is computed as position minus file
// length, the reverse of CCharString::LoadFromFile 0x0099F2E0, so it is never
// positive; retail's bug, kept.
void CWideString::LoadFromFile(CAFile& file, long length)
{
    UnassignString();
    if (length == -1)
        length = file.GetPosition() - file.GetLength();
    storage_ = new CWideStringData(length, L' ');
    if (storage_ != 0)
        file.Read(storage_->text, length * sizeof(wchar_t), 0);
}
