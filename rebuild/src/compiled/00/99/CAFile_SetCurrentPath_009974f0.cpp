#include "fable_filesystem.h"

#include <direct.h>

void FABLE_FASTCALL CAFile::SetCurrentPath(const CWideString& path)
{
    _wchdir(path);
    g_CAFileCurrentPath_013BCA10 = path;
}
