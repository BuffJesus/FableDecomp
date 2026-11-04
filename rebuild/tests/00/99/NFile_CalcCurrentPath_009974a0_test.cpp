#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <direct.h>

#include "fable_filesystem.h"

namespace
{
    wchar_t g_constructed[300];
    int g_constructs;
    const void* g_target;
}

CWideString::CWideString(const wchar_t* text) : storage_(0)
{
    ++g_constructs;
    g_target = this;
    wcsncpy(g_constructed, text, 299);
}
CWideString::CWideString(const CWideString& other) : storage_(other.storage_) {}
CWideString::~CWideString() {}

int main()
{
    wchar_t expected[300];
    _wgetcwd(expected, 300);
    CWideString path = NFile::CalcCurrentPath();

    const bool passed = g_constructs == 1 && g_target == &path && wcscmp(g_constructed, expected) == 0;
    if (!passed)
    {
        printf("FABLETLC_NFILE_CALCCURRENTPATH_009974A0 FAIL\n");
        return 1;
    }
    printf("FABLETLC_NFILE_CALCCURRENTPATH_009974A0_PASS\n");
    return 0;
}