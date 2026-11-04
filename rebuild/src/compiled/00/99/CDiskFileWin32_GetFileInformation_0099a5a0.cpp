// CDiskFileWin32::GetFileInformation @ 0x0099A5A0
// (FableWin ?GetFileInformation@CDiskFileWin32@@UAE?AVCFileInformation@@XZ, slot 15).
//
// Fills the three time stamps and the read-only flag from
// GetFileInformationByHandle; when the query fails, the stamps are reset to
// zeroed dates and the file is reported writable.

#include "fable_disk_file.h"
#include "fable_file_information.h"

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

// winbase.h BY_HANDLE_FILE_INFORMATION.
struct FableByHandleFileInformation
{
    FableDword dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    FableDword dwVolumeSerialNumber;
    FableDword nFileSizeHigh;
    FableDword nFileSizeLow;
    FableDword nNumberOfLinks;
    FableDword nFileIndexHigh;
    FableDword nFileIndexLow;
};

extern "C" __declspec(dllimport) FableBool FABLE_STDCALL GetFileInformationByHandle(
    FableHandle file,
    FableByHandleFileInformation* information);

namespace
{
    const FableDword kFileAttributeReadOnly = 0x00000001;
}

CFileInformation CDiskFileWin32::GetFileInformation()
{
    CFileInformation information;
    FableByHandleFileInformation handleInformation;

    if (GetFileInformationByHandle(WinFileHandle, &handleInformation))
    {
        information.CreationTime.Initialise(handleInformation.ftCreationTime);
        information.LastAccessTime.Initialise(handleInformation.ftLastAccessTime);
        information.LastWriteTime.Initialise(handleInformation.ftLastWriteTime);
        information.ReadOnly = (handleInformation.dwFileAttributes & kFileAttributeReadOnly) != 0;
    }
    else
    {
        information.CreationTime = CDateAndTime();
        information.LastAccessTime = CDateAndTime();
        information.LastWriteTime = CDateAndTime();
        information.ReadOnly = false;
    }

    return information;
}