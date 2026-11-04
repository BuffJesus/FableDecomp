#include <stdio.h>
#include <wchar.h>

#include "fable_resource_directories.h"

// Recorders for the real callees: GetMyDocumentsDir, operator+(wide, literal)
// and ~CWideString. A string's storage word holds its text pointer here.
namespace
{
    wchar_t g_appendStorage[3][260];
    const wchar_t* g_literals[3];
    const void* g_destroyed[4];
    unsigned int g_appendCalls;
    unsigned int g_destroyCalls;

    const wchar_t* TextOf(const CWideString& value)
    {
        return *reinterpret_cast<const wchar_t* const*>(&value);
    }
}

CWideString::CWideString(const wchar_t* text)
{
    *reinterpret_cast<const wchar_t**>(this) = text;
}

CWideString::~CWideString()
{
    if (g_destroyCalls < 4)
        g_destroyed[g_destroyCalls] = this;
    ++g_destroyCalls;
}

CWideString FABLE_FASTCALL NResourceDirectoryNames::GetMyDocumentsDir()
{
    return CWideString(L"C:\\Users\\Hero\\Documents\\");
}

CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    const unsigned int index = g_appendCalls++;
    g_literals[index] = right;
    wcscpy(g_appendStorage[index], TextOf(left));
    wcscat(g_appendStorage[index], right);
    return CWideString(g_appendStorage[index]);
}

int main()
{
    CWideString result = NResourceDirectoryNames::GetSaveDir();
    const unsigned int destroyedBeforeResult = g_destroyCalls;

    // Every intermediate is destroyed inside GetSaveDir; the result is not.
    const bool passed =
        g_appendCalls == 3 &&
        destroyedBeforeResult == 3 &&
        wcscmp(g_literals[0], L"My Games\\Fable\\") == 0 &&
        wcscmp(g_literals[1], L"Saves") == 0 &&
        wcscmp(g_literals[2], L"\\") == 0 &&
        g_destroyed[0] != &result &&
        g_destroyed[1] != &result &&
        g_destroyed[2] != &result &&
        wcscmp(
            TextOf(result),
            L"C:\\Users\\Hero\\Documents\\My Games\\Fable\\Saves\\") == 0;

    if (!passed)
    {
        printf("FABLETLC_SAVE_DIRECTORY_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_SAVE_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
