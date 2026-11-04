// lib_file_abstract.cpp, retail 0x009979E0 (Ego_r
// ?ConvertDirectoryAndFilenameToPath@CAFile@@SI?AVCWideString@@ABV2@0@Z): join
// a directory and a file name with one backslash, after dropping backslashes
// from the start of the name. The directory loop tests the character at
// index GetLength() (the terminator), so it never trims; retail's slip, kept.
// Near match, not gated: retail keeps the return slot in edi and the length in
// esi (swapped here); everything else is exact.
#include "fable_filesystem.h"
#include "fable_string.h"

CWideString FABLE_FASTCALL CAFile::ConvertDirectoryAndFilenameToPath(const CWideString& directory,
                                                                    const CWideString& filename)
{
    CWideString dir(directory);
    CWideString file(filename);
    while (dir.GetAsCharArray()[dir.GetLength()] == L'\\')
    {
        CWideString shorter;
        shorter.SetAsSubString(dir.GetAsCharArray(), dir.GetLength() - 1);
        dir = shorter;
    }
    while (file.GetAsCharArray()[0] == L'\\')
    {
        CWideString shorter;
        shorter.SetAsSubString(file.GetAsCharArray() + 1, file.GetLength() - 1);
        file = shorter;
    }
    return dir + L"\\" + file;
}
