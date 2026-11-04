#include "fable_disk_file.h"

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
extern "C" __declspec(dllimport) FableBool FABLE_STDCALL CloseHandle(
    FableHandle object);

namespace
{
    const FableDword kFileCurrent = 1;
}

// CDiskFileWin32::Close() @ 0x0099A920.
void CDiskFileWin32::Close()
{
    if (Opened)
    {
        SetFilePointer(WinFileHandle, 0, 0, kFileCurrent);
        FlushFileBuffers(WinFileHandle);
        CloseHandle(WinFileHandle);
    }
    Opened = false;
}
