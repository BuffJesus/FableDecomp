// CDiskFileWin32::SetPosition @ 0x0099ABF0
// (FableWin ?SetPosition@CDiskFileWin32@@UAEXJ@Z, vtable slot 5).
//
// A move to the current position is a no-op. Otherwise the Win32 file pointer
// is queried, a seek past the logical length throws BAD_SEEK, and the cached
// position is updated before flushing and seeking the handle from the start.

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

void CDiskFileWin32::SetPosition(long position)
{
    if (Position != position)
    {
        SetFilePointer(WinFileHandle, 0, 0, kFileCurrent);
        if (position > Length)
        {
            CCharString message("Cannot seek past end of file");
            throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
        }

        Position = position;
        FableLargeInteger distance;
        distance.QuadPart = Position;
        FlushFileBuffers(WinFileHandle);
        SetFilePointer(WinFileHandle, distance.LowPart, 0, kFileBegin);
    }
}