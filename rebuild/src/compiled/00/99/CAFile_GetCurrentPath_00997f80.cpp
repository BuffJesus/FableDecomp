// CAFile::GetCurrentPath() @ 0x00997F80 (FableWin ?GetCurrentPath@CAFile@@SI?AVCWideString@@XZ):
// the cached working directory, filled from NFile::CalcCurrentPath the first
// time it is asked for while still empty.
#include "fable_filesystem.h"

CWideString FABLE_FASTCALL CAFile::GetCurrentPath()
{
    if (g_CAFileCurrentPath_013BCA10 == L"")
        g_CAFileCurrentPath_013BCA10 = NFile::CalcCurrentPath();
    return g_CAFileCurrentPath_013BCA10;
}