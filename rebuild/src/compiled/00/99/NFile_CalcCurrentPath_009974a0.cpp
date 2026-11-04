// NFile::CalcCurrentPath() @ 0x009974A0 (Ego_r ?CalcCurrentPath@NFile@@YI?AVCWideString@@XZ):
// the process working directory as a wide string. The unit is built with /GS
// (the path buffer takes the stack cookie).
#include <direct.h>
#include "fable_filesystem.h"

CWideString FABLE_FASTCALL NFile::CalcCurrentPath()
{
    wchar_t path[260];
    _wgetcwd(path, 260);
    return CWideString(path);
}