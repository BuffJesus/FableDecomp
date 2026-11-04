#include "fable_filesystem.h"
#include "fable_threaded_file.h"

// kernel32 (the bootstrap toolchain has no Platform SDK headers).
extern "C" __declspec(dllimport) void* __stdcall CreateFileW(
    const wchar_t* fileName,
    unsigned long desiredAccess,
    unsigned long shareMode,
    void* securityAttributes,
    unsigned long creationDisposition,
    unsigned long flagsAndAttributes,
    void* templateFile);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(
    void* file,
    unsigned long* fileSizeHigh);

// Retail keeps independent write (013BC9E8) and read (013BC9EC) sequences.
static unsigned long s_NextPhysicalSortKey;
static unsigned long s_NextReadPhysicalSortKey;

// CThreadedFile::Open(CWideString const&, bool) @ 0x0098E1E0.
bool CThreadedFile::Open(const CWideString& filename, bool noCaching)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(filename);
    CWideString drive;
    CAFile::SplitPath(fullPath, &drive, 0, 0, 0);

    openedForWrite_ = false;
    const bool onDvd = drive.ToCharString().GetAsUpperCase() == "D:";
    deviceId_ = onDvd ? 1 : 0;

    // FILE_FLAG_OVERLAPPED | FILE_ATTRIBUTE_READONLY [| FILE_FLAG_NO_BUFFERING]
    unsigned long flags = 0x40000001UL;
    if (noCaching)
        flags = 0x60000001UL;

    // GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING
    fileHandle_ = CreateFileW(
        fullPath, 0x80000000UL, 1, 0, 3, flags, 0);
    if (fileHandle_ == reinterpret_cast<void*>(-1))
        return false;

    filenameStorage_ = filename;
    length_ = GetFileSize(fileHandle_, 0);
    physicalSortKey_ = ++s_NextReadPhysicalSortKey;
    openFlag_ = true;
    return true;
}


bool CThreadedFile::OpenForWrite(const CWideString& filename, bool overwrite)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(filename);
    deviceId_ = 0;
    fileHandle_ = CreateFileW(fullPath,0x40000000UL,0,0,overwrite ? 2 : 4,0x68000000UL,0);
    if (fileHandle_ == reinterpret_cast<void*>(-1))
        return false;
    length_ = GetFileSize(fileHandle_,0);
    physicalSortKey_ = ++s_NextPhysicalSortKey;
    openedForWrite_ = true;
    openFlag_ = true;
    return true;
}
