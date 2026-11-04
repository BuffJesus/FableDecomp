// lib_file_abstract.cpp, retail 0x00999110 (Ego_r
// ?ConvertPathToFullPath@CAFile@@SI?AVCWideString@@ABV2@@Z): the path expanded
// against the current path. This replaces an earlier stand-in that modelled
// the second ExpandPath argument as a default-constructed scratch string;
// retail passes GetCurrentPath() (0x00997F80).
#include "fable_filesystem.h"
#include "fable_string.h"

CWideString FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CWideString& path)
{
    return ExpandPath(path, GetCurrentPath());
}
