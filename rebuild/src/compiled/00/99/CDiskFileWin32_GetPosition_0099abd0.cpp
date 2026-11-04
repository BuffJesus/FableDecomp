// CDiskFileWin32::GetPosition @ 0x0099ABD0
// (FableWin ?GetPosition@CDiskFileWin32@@UBEJXZ, vtable slot 7).
//
// Touches the Win32 file pointer (a zero FILE_CURRENT move whose result is
// discarded) and returns the cached logical position.

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
    const FableDword kFileCurrent = 1;
}

long CDiskFileWin32::GetPosition() const
{
    SetFilePointer(WinFileHandle, 0, 0, kFileCurrent);
    return Position;
}
