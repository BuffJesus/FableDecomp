// CDiskFileWin32::MovePosition @ 0x0099AC90
// (FableWin ?MovePosition@CDiskFileWin32@@UAEXJ@Z, vtable slot 6).
//
// Queries the Win32 file pointer, rejects a relative move that would land
// before the start or past the logical length (BAD_SEEK), then caches the new
// position, flushes and seeks the handle from the start of the file.

#include "fable_disk_file.h"
#include "fable_file_exception.h"

typedef int FableBool;
typedef unsigned long FableDword;
typedef void* FableHandle;

extern "C" __declspec(dllimport) FableDword FABLE_STDCALL SetFilePointer(
    FableHandle file,
    long distanceToMove,
    long* distanceToMoveHigh,
    FableDword moveMethod);
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL FlushFileBuffers(
    FableHandle file);

// winnt.h LARGE_INTEGER.
union FableLargeInteger
{
    struct
    {
        FableDword LowPart;
        long HighPart;
    };
    __int64 QuadPart;
};

namespace
{
    const FableDword kFileBegin = 0;
    const FableDword kFileCurrent = 1;
}

void CDiskFileWin32::MovePosition(long offset)
{
    SetFilePointer(WinFileHandle, 0, 0, kFileCurrent);

    if (Position + offset < 0)
    {
        CCharString message("Cannot move before beginning of file");
        throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
    }
    if (Position + offset > Length)
    {
        CCharString message("Cannot seek past end of file");
        throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
    }

    Position += offset;
    FableLargeInteger distance;
    distance.QuadPart = Position;
    FlushFileBuffers(WinFileHandle);
    SetFilePointer(WinFileHandle, distance.LowPart, 0, kFileBegin);
}