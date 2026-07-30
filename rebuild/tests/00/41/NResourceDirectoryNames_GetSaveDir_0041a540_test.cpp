#include <stdio.h>
#include <wchar.h>

#include "fable_resource_directories.h"

namespace
{
    wchar_t g_appendStorage[3][260];
    const wchar_t* g_literals[3];
    void* g_destroyed[3];
    unsigned int g_appendCalls;
    unsigned int g_destroyCalls;
}

extern "C" void* FABLE_FASTCALL
FableGetMyDocumentsDir_0041A540(
    void* destination,
    void*)
{
    *static_cast<const wchar_t**>(destination) =
        L"C:\\Users\\Hero\\Documents\\";
    return destination;
}

extern "C" void* FABLE_FASTCALL
FableAppendWideLiteral_0041A540(
    void* destination,
    void* left,
    const wchar_t* right)
{
    const unsigned int index = g_appendCalls++;
    g_literals[index] = right;
    wcscpy(
        g_appendStorage[index],
        *static_cast<const wchar_t* const*>(left));
    wcscat(g_appendStorage[index], right);
    *static_cast<const wchar_t**>(destination) =
        g_appendStorage[index];
    return destination;
}

extern "C" void FABLE_FASTCALL
FableDestroyWideTemporary_0041A540(
    void* value,
    void*)
{
    if (g_destroyCalls < 3)
    {
        g_destroyed[g_destroyCalls] = value;
    }
    ++g_destroyCalls;
}

CWideString::~CWideString()
{
}

int main()
{
    CWideString result =
        NResourceDirectoryNames::GetSaveDir();
    const wchar_t* text =
        *reinterpret_cast<const wchar_t* const*>(&result);

    const bool passed =
        g_appendCalls == 3 &&
        g_destroyCalls == 3 &&
        wcscmp(g_literals[0], L"My Games\\Fable\\") == 0 &&
        wcscmp(g_literals[1], L"Saves") == 0 &&
        wcscmp(g_literals[2], L"\\") == 0 &&
        wcscmp(
            text,
            L"C:\\Users\\Hero\\Documents\\My Games\\Fable\\Saves\\") ==
                0;

    if (!passed)
    {
        printf("FABLETLC_SAVE_DIRECTORY_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_SAVE_DIRECTORY_BEHAVIOR PASS\n");
    return 0;
}
