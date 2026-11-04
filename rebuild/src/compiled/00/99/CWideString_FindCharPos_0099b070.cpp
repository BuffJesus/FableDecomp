#include <wchar.h>
#include "fable_string.h"

// lib_wide_string.cpp, retail 0099B070 (Ego_r parameter ch): the index of the
// first ch, or -1. The text pointer is read twice (once for the search, once
// for the offset).
long CWideString::FindCharPos(wchar_t ch) const
{
    const wchar_t* found = wcschr(storage_ == 0 ? L"" : storage_->text, ch);
    if (found != 0)
        return found - (storage_ == 0 ? L"" : storage_->text);
    return -1;
}
