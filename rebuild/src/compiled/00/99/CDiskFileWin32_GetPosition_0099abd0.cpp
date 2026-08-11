// CDiskFileWin32::GetPosition @ 0x0099ABD0.
//
// Queries the current Win32 file pointer (via SetFilePointer with a zero move
// and FILE_CURRENT) but discards the API result, returning the cached logical
// position stored at this+0x08.
//
// The SetFilePointer call is emitted as an indirect call through the IAT slot,
// modeled here as a function-pointer global.

typedef unsigned long DWORD_0099abd0;

extern "C" DWORD_0099abd0 (__stdcall *CDiskFileWin32_SetFilePointerIAT_0099abd0)(
    void* hFile,
    long lDistanceToMove,
    long* lpDistanceToMoveHigh,
    DWORD_0099abd0 dwMoveMethod);

struct CDiskFileWin32_0099abd0
{
    char pad0[8];
    long position; // +0x08
    char pad2[4];  // +0x0C
    void* handle;  // +0x10
};

extern "C" long __fastcall
GetPosition_0099abd0(CDiskFileWin32_0099abd0* self)
{
    CDiskFileWin32_SetFilePointerIAT_0099abd0(self->handle, 0, 0, 1 /*FILE_CURRENT*/);
    return self->position;
}