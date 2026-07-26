#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "fable_filesystem.h"

typedef unsigned long FableDword;
typedef void* FableModuleHandle;

extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetModuleFileNameW(
    FableModuleHandle module,
    wchar_t* filename,
    FableDword size);

namespace
{
    const fable_u32 kMaximumPathCharacters = 260;
    wchar_t g_ownedText[kMaximumPathCharacters];
}

CWideString::CWideString(const wchar_t* text)
{
    wcsncpy(g_ownedText, text, kMaximumPathCharacters - 1);
    g_ownedText[kMaximumPathCharacters - 1] = L'\0';
    storage_ = reinterpret_cast<CWideStringData*>(g_ownedText);
}

CWideString::~CWideString()
{
    storage_ = 0;
}

CWideString::operator const wchar_t*() const
{
    return reinterpret_cast<const wchar_t*>(storage_);
}

static int VerifyExecutableDirectoryIsReturned()
{
    wchar_t executablePath[kMaximumPathCharacters];
    wchar_t expectedDrive[10];
    wchar_t expectedDirectory[kMaximumPathCharacters + 2];
    wchar_t expectedPath[kMaximumPathCharacters];

    GetModuleFileNameW(0, executablePath, kMaximumPathCharacters);
    _wsplitpath(
        executablePath,
        expectedDrive,
        expectedDirectory,
        0,
        0);
    _snwprintf(
        expectedPath,
        kMaximumPathCharacters,
        L"%s%s",
        expectedDrive,
        expectedDirectory);
    expectedPath[kMaximumPathCharacters - 1] = L'\0';

    const CWideString projectPath = CAFile::GetProjectPath();
    const wchar_t* actualPath = projectPath;

    if (actualPath == 0)
        return 1;
    if (wcscmp(actualPath, expectedPath) != 0)
        return 2;

    const size_t length = wcslen(actualPath);
    if (length == 0)
        return 3;
    if (actualPath[length - 1] != L'\\' && actualPath[length - 1] != L'/')
        return 4;
    return 0;
}

int main()
{
    const int result = VerifyExecutableDirectoryIsReturned();
    if (result != 0)
    {
        printf("FABLETLC_GET_PROJECT_PATH_BEHAVIOR FAIL code=%d\n", result);
        return result;
    }

    printf("FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS\n");
    return 0;
}
