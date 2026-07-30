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
        CWideString value = FableGetGraphicsDirectory_0041A100();
        if (g_constructedText == 0 ||
            wcscmp(g_constructedText, L"Data\\Graphics\\") != 0)
        {
            printf("FABLETLC_GRAPHICS_DIRECTORY_BEHAVIOR FAIL\n");
            return 1;
        }
    }
    printf("FABLETLC_GRAPHICS_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
