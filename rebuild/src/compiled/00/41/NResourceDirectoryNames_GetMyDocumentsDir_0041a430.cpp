#include "fable_resource_directories.h"
#include "fable_shell_paths.h"

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
