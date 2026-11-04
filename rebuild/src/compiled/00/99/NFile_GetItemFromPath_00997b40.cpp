// lib_file_abstract.cpp, retail 0x00997B40 (/GS; Ego_r local t_out[260]): the
// CWideString form of NFile::GetItemFromPath, through the character form
// (0x00997880, called out of line).
#include "fable_filesystem.h"
#include "fable_string.h"

bool FABLE_FASTCALL NFile::GetItemFromPath(const CWideString& pathname, unsigned long directory_number,
                                           CWideString& directory_name)
{
    wchar_t t_out[260];
    if (GetItemFromPath(pathname.GetAsCharArray(), directory_number, t_out))
    {
        directory_name = t_out;
        return true;
    }
    return false;
}
