#include <stdio.h>
#include <wchar.h>

#include "fable_resource_directories.h"

namespace
{
    const wchar_t* g_constructedText = 0;
}

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

CWideString::CWideString(const wchar_t* text)
{
    g_constructedText = text;
}

CWideString::~CWideString()
{
}

int main()
{
    {
        CWideString value = FableGetCacheDirectory_0041A180();
        if (g_constructedText == 0 ||
            wcscmp(g_constructedText, L"Data\\Cache\\") != 0)
        {
            puts("FABLETLC_CACHE_DIRECTORY_BEHAVIOR FAIL");
            return 1;
        }
    }

    puts("FABLETLC_CACHE_DIRECTORY_BEHAVIOR PASS");
    return 0;
}
