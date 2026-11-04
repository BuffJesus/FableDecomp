#include "fable_system_manager.h"
#include "fable_filesystem.h"

extern "C" __declspec(dllimport) int __stdcall RemoveDirectoryW(const wchar_t* pathName);
extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar_t* fileName);
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
extern "C" int __cdecl _wchdir(const wchar_t* dirname);
extern "C" void __cdecl _wsplitpath(const wchar_t* path, wchar_t* drive, wchar_t* dir,
                                    wchar_t* fname, wchar_t* ext);

// lib_drive_manager.cpp is built with /GS (retail stack cookies on the array locals).

// Retail 009E4AE0: probe the drive by changing to its root, then restore the directory.
EDriveReturn CDriveManager::DriveExists(wchar_t drive)
{
    CWideString cdir = CAFile::GetCurrentPath();
    wchar_t pathname[4];
    pathname[0] = drive;
    pathname[1] = L':';
    pathname[2] = L'\\';
    pathname[3] = 0;
    if (_wchdir(pathname) == 0) {
        _wchdir(cdir);
        return DR_OK;
    }
    return DR_ERROR_DRIVE_NOT_FOUND;
}

// lib_drive_manager.cpp. Retail 009E4B70; the error code is read and discarded.
EDriveReturn CDriveManager::DeleteDirectory(const CWideString& pathName)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(pathName);
    if (RemoveDirectoryW(fullPath))
        return DR_OK;
    GetLastError();
    return DR_ERROR_UNKNOWN;
}

// Retail 009E4BC0 (manifest label DeleteDirectory is wrong; it calls DeleteFileW).
EDriveReturn CDriveManager::DeleteFileW(const CWideString& pathName)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(pathName);
    if (::DeleteFileW(fullPath))
        return DR_OK;
    GetLastError();
    return DR_ERROR_UNKNOWN;
}

// Retail 009E4C70.
wchar_t CDriveManager::GetDriveFromPath(const wchar_t* pathname)
{
    wchar_t drive[3];
    _wsplitpath(pathname, drive, 0, 0, 0);
    return drive[0];
}

