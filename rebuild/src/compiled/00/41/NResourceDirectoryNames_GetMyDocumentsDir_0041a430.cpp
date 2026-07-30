#include "fable_resource_directories.h"

typedef long (FABLE_STDCALL *FableGetFolderPathW_0041A430)(
    void* window,
    int folder,
    void* token,
    fable_u32 flags,
    wchar_t* path);

extern "C" FableGetFolderPathW_0041A430
    g_FableGetFolderPathW_0041A430;

CWideString FABLE_FASTCALL
NResourceDirectoryNames::GetMyDocumentsDir()
{
    wchar_t path[260];
    g_FableGetFolderPathW_0041A430(
        0,
        5,
        0,
        0,
        path);

    CWideString directory(path);
    long lastIndex = directory.GetLength() - 1;
    const wchar_t* text = directory;
    if (text[lastIndex] != L'\\')
    {
        directory += L"\\";
    }
    return directory;
}
