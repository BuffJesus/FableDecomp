// lib_file_abstract.cpp, retail 0x00999760 / 0x00999850 (/GS; Ego_r locals):
// CAFile::StartFileScan and ScanNextFile, the wchar_t* forms. Each expands
// the pattern against the current path and runs _wfindfirst/_wfindnext. The
// match comes back as the pattern's drive and directory plus the found name.
// The search closes itself when _wfindnext fails.
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "fable_filesystem.h"
#include "fable_string.h"

// Retail 00999760.
bool FABLE_FASTCALL CAFile::StartFileScan(const CWideString& pathname, CFileScanInfo& info, wchar_t* found_file)
{
    wchar_t drive[3];
    wchar_t directory[256];
    CWideString fullpath = ExpandPath(pathname, GetCurrentPath());
    _wfinddata_t file_info;
    info.SearchHandle = _wfindfirst(fullpath.GetAsCharArray(), &file_info);
    if (info.SearchHandle != -1)
    {
        _wsplitpath(fullpath, drive, directory, 0, 0);
        wcscpy(found_file, drive);
        wcscat(found_file, directory);
        wcscat(found_file, file_info.name);
        info.Stopped = false;
        return true;
    }
    info.Stopped = true;
    return false;
}

// Retail 00999850.
bool FABLE_FASTCALL CAFile::ScanNextFile(const CWideString& pathname, CFileScanInfo& info, wchar_t* found_file)
{
    wchar_t drive[3];
    wchar_t directory[256];
    CWideString fullpath = ExpandPath(pathname, GetCurrentPath());
    _wfinddata_t file_info;
    if (!info.Stopped)
    {
        if (_wfindnext(info.SearchHandle, &file_info) == 0)
        {
            _wsplitpath(fullpath, drive, directory, 0, 0);
            wcscpy(found_file, drive);
            wcscat(found_file, directory);
            wcscat(found_file, file_info.name);
            return true;
        }
        info.Stopped = true;
        _findclose(info.SearchHandle);
    }
    return false;
}
