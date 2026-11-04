// Behaviour: GetLevelDir constructs its result (hidden return slot) from the literal
// L"Data\\Levels\\" exactly once, and returns that slot.
#include "fable_resource_directories.h"
#include <stdio.h>
#include <wchar.h>

static int g_ctor; static const void* g_this; static const wchar_t* g_text;
CWideString::CWideString(const wchar_t* text) { ++g_ctor; g_this = this; g_text = text; }
CWideString::~CWideString() {}

int main()
{
    g_ctor = 0;
    {
        CWideString result = NResourceDirectoryNames::GetLevelDir();
        if (g_ctor != 1 || g_this != &result || g_text == 0 || wcscmp(g_text, L"Data\\Levels\\") != 0) { printf("FAIL\n"); return 1; }
    }
    printf("0041a060_TEST PASS\n");
    return 0;
}