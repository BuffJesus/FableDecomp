#include <new>
#include "fable_disk_file.h"

extern CCharString FableFullBuildVersion;

void FABLE_CDECL GFGetBuildNumber2()
{
    bool exists;
    {
        CWideString path(L"full_build_version.txt");
        exists = CAFile::PathExists(path);
    }
    if (!exists)
        return;

    unsigned char fileStorage[sizeof(CDiskFileWin32)];
    {
        CWideString path(L"full_build_version.txt");
        new (fileStorage) CDiskFileWin32(path, FILE_ACCESS_READ, 2);
    }
    {
        CCharString contents(
            reinterpret_cast<CDiskFileWin32*>(fileStorage), -1);
        FableFullBuildVersion = contents;
    }
    reinterpret_cast<CDiskFileWin32*>(fileStorage)
        ->CDiskFileWin32::~CDiskFileWin32();
}
