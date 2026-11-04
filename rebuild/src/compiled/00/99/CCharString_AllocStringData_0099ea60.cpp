// CCharString::AllocStringData(char const*, long) @ 0x0099EA60.
#include "fable_string.h"

// A new, singly-owned data block holding a copy of the text: the whole C
// string when length is -1, otherwise its first `length` characters.
CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    CCharStringData* data;
    if (length == -1)
        data = new CCharStringData(text);
    else
        data = new CCharStringData(text, length);
    return data;
}
