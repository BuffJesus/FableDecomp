#ifndef _DLL
#define _DLL
#endif
#include <wchar.h>

extern "C" void __cdecl wsplitpath(const wchar_t *path, wchar_t *drive, wchar_t *dir, wchar_t *fname, wchar_t *ext)
{
    _wsplitpath(path, drive, dir, fname, ext);
}