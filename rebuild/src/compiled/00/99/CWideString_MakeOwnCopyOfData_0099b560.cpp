// lib_wide_string.cpp, retail 0x0099B560-0x0099B770: copy-on-write helpers
// and SetAsSubString. A shared buffer is copied before it is modified; an
// empty string becomes a zero-length buffer; UnassignString is expanded into
// SetAsSubString, as in the other lib_wide_string units.
#include <stdlib.h>
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

// Defined earlier in the same file (out of line at 0x0099B3C0); the copies
// below expand it.
inline CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    CWideStringData* data;
    if (length == -1)
        data = new CWideStringData(text);
    else
        data = new CWideStringData(text, length);
    return data;
}

// Retail 0099B560.
void CWideString::MakeOwnCopyOfData()
{
    if (storage_ != 0 && storage_->owners > 1)
    {
        CWideStringData* copy = AllocStringData(storage_->Data.c_str(), -1);
        UnassignString();
        storage_ = copy;
    }
}

// Retail 0099B5E0.
void CWideString::MakeOwnCopyOfDataOrEmptyString()
{
    if (storage_ != 0)
    {
        if (storage_->owners > 1)
        {
            CWideStringData* copy = AllocStringData(storage_->Data.c_str(), -1);
            UnassignString();
            storage_ = copy;
        }
    }
    else
        storage_ = new CWideStringData(0, L' ');
}

// Retail 0099B770.
void CWideString::SetAsSubString(const wchar_t* text, long length)
{
    UnassignString();
    if (text != 0 && *text != L'\0')
        storage_ = AllocStringData(text, length);
}
