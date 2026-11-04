#include <stdlib.h>
#include "fable_string.h"

// lib_wide_string.cpp: operator!=(const CWideString&) 0099B0F0 and
// operator=(wchar_t) 0099B860. UnassignString is expanded into the
// assignment; it is defined inline here as in the operator+ units.
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

// Retail 0099B0F0.
bool CWideString::operator!=(const CWideString& other) const
{
    return !(*this == other);
}

// Retail 0099B860: a one-character string, or empty for L'\0'.
const CWideString& CWideString::operator=(wchar_t character)
{
    wchar_t text[2];
    text[0] = character;
    text[1] = 0;
    UnassignString();
    if (text[0] != 0)
        storage_ = AllocStringData(text, -1);
    return *this;
}
