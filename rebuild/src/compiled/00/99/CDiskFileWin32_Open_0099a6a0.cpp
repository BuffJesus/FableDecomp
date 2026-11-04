// CDiskFileWin32::Open @ 0x0099A6A0
// (FableWin ?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z, slot 1).
//
// Remembers the path, expands it to a full path (BAD_PARAMETERS when that is
// empty), then opens it through CreateFileW for the access type: reading
// shares reads and needs the file; read/write checks the drive is writeable;
// read/write/create opens or creates it; create makes the directory and
// truncates. Both creating modes then count as read/write. Flag 2 asks for
// sequential scanning. A failed open throws FILE_NOT_FOUND with the Windows
// error; an unknown access type leaves the handle null. The file starts at
// position 0 with its size cached.

#include "fable_disk_file.h"
#include "fable_file_exception.h"

typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableHandle FABLE_STDCALL CreateFileW(
    const wchar_t* fileName,
    FableDword desiredAccess,
    FableDword shareMode,
    void* securityAttributes,
    FableDword creationDisposition,
    FableDword flagsAndAttributes,
    FableHandle templateFile);
extern "C" __declspec(dllimport) FableDword FABLE_STDCALL GetFileSize(
    FableHandle file,
    FableDword* fileSizeHigh);
extern "C" __declspec(dllimport) FableDword FABLE_STDCALL SetFilePointer(
    FableHandle file,
    long distanceToMove,
    long* distanceToMoveHigh,
    FableDword moveMethod);

namespace
{
    const FableDword kGenericRead = 0x80000000;
    const FableDword kGenericWrite = 0x40000000;
    const FableDword kFileShareRead = 1;
    const FableDword kCreateAlways = 2;
    const FableDword kOpenExisting = 3;
    const FableDword kOpenAlways = 4;
    const FableDword kFileFlagSequentialScan = 0x08000000;
    const FableDword kFileBegin = 0;
    const FableDword kFileCurrent = 1;
    const FableHandle kInvalidHandle = reinterpret_cast<FableHandle>(-1);
}

void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    CWideString fullPathname;
    FableDword flagsAndAttributes = 0;
    Pathname = pathname;
    WinFileHandle = 0;
    if (flags & 2)
        flagsAndAttributes = kFileFlagSequentialScan;

    ConvertPathToFullPath(pathname, fullPathname);
    if (fullPathname.GetLength() <= 0)
    {
        CCharString message = "Bad filename " + fullPathname.ToCharString();
        throw CBBBFileException(message, CBBBFileException::BAD_PARAMETERS);
    }

    FableHandle handle;
    switch (accessType)
    {
    case FILE_ACCESS_READ:
        handle = CreateFileW(fullPathname, kGenericRead, kFileShareRead, 0,
                             kOpenExisting, flagsAndAttributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE:
        ValidatePathnameIsOnWriteableDrive(fullPathname);
        handle = CreateFileW(fullPathname, kGenericRead | kGenericWrite, kFileShareRead, 0,
                             kOpenExisting, flagsAndAttributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE_CREATE:
        ValidatePathnameIsOnWriteableDrive(fullPathname);
        handle = CreateFileW(fullPathname, kGenericRead | kGenericWrite, 0, 0,
                             kOpenAlways, flagsAndAttributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    case FILE_ACCESS_CREATE:
        ValidatePathnameIsOnWriteableDrive(fullPathname);
        CreateDirectoryForFile(fullPathname);
        handle = CreateFileW(fullPathname, kGenericRead | kGenericWrite, 0, 0,
                             kCreateAlways, flagsAndAttributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    default:
        handle = 0;
        break;
    }

    if (handle == kInvalidHandle)
    {
        CCharString error = GFGetLastWindowsError();
        CCharString message = "Cannot open file '" + fullPathname.ToCharString() + "' (access type " +
                              GFIntToCharString(accessType) + ") - " + error;
        throw CBBBFileException(message, CBBBFileException::FILE_NOT_FOUND);
    }

    Length = GetFileSize(handle, 0);
    SetFilePointer(handle, 0, 0, kFileBegin);
    WinFileHandle = handle;
    Position = 0;
    Opened = true;
    SetFilePointer(WinFileHandle, 0, 0, kFileCurrent);
}