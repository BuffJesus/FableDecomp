#include "fable_filesystem.h"

#include <stdio.h>
#include <stdlib.h>

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableModuleHandle;

extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetModuleFileNameW(
    FableModuleHandle module,
    wchar_t* filename,
    FableDword size);

namespace
{
    const fable_u32 kMaximumPathCharacters = 260;
    const fable_u32 kDriveBufferCharacters = 10;
    const fable_u32 kDirectoryBufferCharacters =
        kMaximumPathCharacters + 2;
}

CWideString FABLE_FASTCALL CAFile::GetProjectPath()
{
    wchar_t modulePath[kMaximumPathCharacters];
    wchar_t drive[kDriveBufferCharacters];
    wchar_t directory[kDirectoryBufferCharacters];

    GetModuleFileNameW(0, modulePath, kMaximumPathCharacters);
    _wsplitpath(modulePath, drive, directory, 0, 0);
    _snwprintf(
        modulePath,
        kMaximumPathCharacters,
        L"%s%s",
        drive,
        directory);
    modulePath[kMaximumPathCharacters - 1] = L'\0';

    return CWideString(modulePath);
}
