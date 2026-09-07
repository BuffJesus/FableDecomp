// CDiskFileWin32::GetPosition @ 0x0099ABD0.
//
// Queries the current Win32 file pointer (via SetFilePointer with a zero move
// and FILE_CURRENT) but discards the API result, returning the cached logical
// position stored at this+0x08.
//
// The SetFilePointer call is emitted as an indirect call through the IAT slot,
// modeled here as a function-pointer global.

#include "engine/CDiskFileWin32.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned long DWORD_0099abd0;

extern "C" DWORD_0099abd0 (__stdcall *CDiskFileWin32_SetFilePointerIAT_0099abd0)(
    void* hFile,
    long lDistanceToMove,
    long* lpDistanceToMoveHigh,
    DWORD_0099abd0 dwMoveMethod);


extern "C" long __fastcall
GetPosition_0099abd0(CDiskFileWin32* self)
{
    CDiskFileWin32_SetFilePointerIAT_0099abd0(self->WinFileHandle, 0, 0, 1 /*FILE_CURRENT*/);
    return self->Position;
}