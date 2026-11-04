// CDiskFileWin32::SetLength @ 0x0099A560
// (FableWin ?SetLength@CDiskFileWin32@@UAEXJ@Z, vtable slot 8).
//
// Only grows the file: a larger length is cached, the Win32 pointer is moved
// to it (extending the file on the next write) and then returned to the
// cached position.

#include "fable_disk_file.h"

typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableDword FABLE_STDCALL SetFilePointer(
    FableHandle file,
    long distanceToMove,
    long* distanceToMoveHigh,
    FableDword moveMethod);

namespace
{
    const FableDword kFileBegin = 0;
}

void CDiskFileWin32::SetLength(long length)
{
    if (length > Length)
    {
        Length = length;
        SetFilePointer(WinFileHandle, length, 0, kFileBegin);
        SetFilePointer(WinFileHandle, Position, 0, kFileBegin);
    }
}