// lib_file_abstract.cpp, retail 0x00999C50 / 0x00999CB0 (/GS): the CWideString
// forms of CAFile::StartFileScan and ScanNextFile, through the wchar_t* forms
// (0x00999760 / 0x00999850, called out of line). Ego_r locals: tfilename[261].
#include "fable_filesystem.h"
#include "fable_string.h"

// Retail 00999C50.
bool FABLE_FASTCALL CAFile::StartFileScan(const CWideString& pathname, CFileScanInfo& info, CWideString& found_file)
{
    wchar_t tfilename[261];
    if (StartFileScan(pathname, info, tfilename))
    {
        found_file = tfilename;
        return true;
    }
    return false;
}

// Retail 00999CB0.
bool FABLE_FASTCALL CAFile::ScanNextFile(const CWideString& pathname, CFileScanInfo& info, CWideString& found_file)
{
    wchar_t tfilename[261];
    if (ScanNextFile(pathname, info, tfilename))
    {
        found_file = tfilename;
        return true;
    }
    return false;
}
