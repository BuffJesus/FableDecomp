// lib_file_abstract.cpp, retail 0x009990B0-0x0099949F and 0x00999A30: path and attribute
// queries that resolve their argument against the current path through
// CAFile::ExpandPath first (Ego_r names).
#include "fable_filesystem.h"
#include "fable_file_information.h"
#include "fable_string.h"

// WIN32_FILE_ATTRIBUTE_DATA, and the kernel32 imports (this tree has no
// <windows.h>; CAFile_GetFileModifiedTime_009992a0.cpp does the same).
struct FableFileAttributeData
{
    unsigned long Attributes;
    FILETIME CreationTime;
    FILETIME LastAccessTime;
    FILETIME LastWriteTime;
    unsigned long FileSizeHigh;
    unsigned long FileSizeLow;
};
extern "C" __declspec(dllimport) int __stdcall GetFileAttributesExW(const wchar_t*, int, void*);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileAttributesW(const wchar_t*);
extern "C" __declspec(dllimport) int __stdcall SetFileAttributesW(const wchar_t*, unsigned long);

// Retail 009990B0.
CCharString FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CCharString& path)
{
    CWideString wide_path = path.ToWideString();
    CWideString full_path = ExpandPath(wide_path, GetCurrentPath());
    return full_path.ToCharString();
}

// Retail 00999320: ftLastWriteTime as one 64-bit value.
unsigned __int64 FABLE_FASTCALL CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString& pathname)
{
    CWideString full_pathname;
    full_pathname = ExpandPath(pathname, GetCurrentPath());
    FableFileAttributeData data;
    GetFileAttributesExW(full_pathname, 0, &data);
    return (unsigned __int64)data.LastWriteTime.dwHighDateTime * 0x100000000ui64 +
           data.LastWriteTime.dwLowDateTime;
}

// Retail 009993B0.
bool FABLE_FASTCALL CAFile::IsFileReadOnly(const CWideString& pathname)
{
    CWideString full_pathname;
    full_pathname = ExpandPath(pathname, GetCurrentPath());
    return (GetFileAttributesW(full_pathname) & 1) != 0;  // FILE_ATTRIBUTE_READONLY
}

// Retail 00999420: set or clear FILE_ATTRIBUTE_READONLY.
void FABLE_FASTCALL CAFile::SetFileReadOnly(const CWideString& pathname, bool read_only)
{
    CWideString full_pathname;
    full_pathname = ExpandPath(pathname, GetCurrentPath());
    unsigned long attributes = GetFileAttributesW(full_pathname);
    if (read_only)
        attributes |= 1;
    else
        attributes &= ~1ul;
    SetFileAttributesW(full_pathname, attributes);
}

// Retail 00999A30: an existing path that is not a directory.
bool FABLE_FASTCALL CAFile::IsPathFile(const CWideString& pathname)
{
    CWideString full_pathname;
    full_pathname = ExpandPath(pathname, GetCurrentPath());
    if (PathExists(pathname) && !(GetFileAttributesW(full_pathname) & 0x10))  // FILE_ATTRIBUTE_DIRECTORY
        return true;
    return false;
}
