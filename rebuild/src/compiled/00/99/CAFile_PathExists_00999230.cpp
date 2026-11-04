#include "fable_filesystem.h"

typedef unsigned long FableDword;

extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetFileAttributesW(
    const wchar_t* filename);

namespace
{
    const FableDword kInvalidFileAttributes = 0xFFFFFFFFul;
}

// CAFile::PathExists(CWideString const&) @ 0x00999230.
bool FABLE_FASTCALL CAFile::PathExists(const CWideString& pathname)
{
    CWideString fullPathname;
    fullPathname = ExpandPath(pathname, GetCurrentPath());
    if (GetFileAttributesW(fullPathname) != kInvalidFileAttributes)
    {
        return true;
    }
    return false;
}
