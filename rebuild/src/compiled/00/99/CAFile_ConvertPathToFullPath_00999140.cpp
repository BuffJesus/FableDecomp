// CAFile::ConvertPathToFullPath(const CWideString&, CWideString&) @ 0x00999140
// (FableWin ?ConvertPathToFullPath@CAFile@@SI_NABVCWideString@@AAV2@@Z): the
// path expanded against the current directory; always succeeds.
#include "fable_filesystem.h"

bool FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CWideString& path, CWideString& fullPath)
{
    fullPath = ExpandPath(path, GetCurrentPath());
    return true;
}