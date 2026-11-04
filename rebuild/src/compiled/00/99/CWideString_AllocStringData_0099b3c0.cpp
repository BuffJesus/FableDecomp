// CWideString::AllocStringData(wchar_t const*, long) @ 0x0099B3C0.
#include "fable_string.h"

// A new, singly-owned data block holding a copy of the text: the whole C
// string when length is -1, otherwise its first `length` characters.
CWideStringData* CWideString::AllocStringData(const wchar_t* text, long length)
{
    CWideStringData* data;
    if (length == -1)
        data = new CWideStringData(text);
    else
        data = new CWideStringData(text, length);
    return data;
}