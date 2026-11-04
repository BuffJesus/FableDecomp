// lib_file_abstract.cpp, retail 0x00998C30 (FableWin
// ?GetAbsoluteFilePath@CAFile@@SI?AVCWideString@@ABV2@0@Z): the wide form,
// through the narrow one (0x00998250).
#include "fable_filesystem.h"
#include "fable_string.h"

CWideString FABLE_FASTCALL CAFile::GetAbsoluteFilePath(const CWideString& directory, const CWideString& path)
{
    return GetAbsoluteFilePath(directory.ToCharString(), path.ToCharString()).ToWideString();
}
