#include "rebuild_abi.h"

#pragma pack(push, 1)
struct FableDiskFileWin32_0099A920
{
    void** vtable;
    fable_u32 accessType;
    fable_i32 position;
    fable_i32 length;
    void* winFileHandle;
    fable_u8 opened;
    fable_u8 padding15[3];
    void* pathnameStorage;
};
#pragma pack(pop)

typedef unsigned long (__stdcall *FableSetFilePointerProc)(
    void*,
    long,
    long*,
    unsigned long);
typedef int (__stdcall *FableHandleProc)(void*);

extern "C" FableSetFilePointerProc
g_FableSetFilePointer_0143FDE8;
extern "C" FableHandleProc
g_FableFlushFileBuffers_0143FDA8;
extern "C" FableHandleProc
g_FableCloseHandle_0143FE30;

FABLE_STATIC_ASSERT(sizeof(FableDiskFileWin32_0099A920) == 0x1C);

// CDiskFileWin32::Close() @ 0x0099A920.
extern "C" __declspec(naked) void FABLE_FASTCALL
FableDiskFileWin32Close_0099A920(
    FableDiskFileWin32_0099A920*,
    void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov al, byte ptr [esi + 14h]
        test al, al
        je closed
        mov eax, dword ptr [esi + 10h]
        push 1
        push 0
        push 0
        push eax
        call dword ptr [g_FableSetFilePointer_0143FDE8]
        mov ecx, dword ptr [esi + 10h]
        push ecx
        call dword ptr [g_FableFlushFileBuffers_0143FDA8]
        mov edx, dword ptr [esi + 10h]
        push edx
        call dword ptr [g_FableCloseHandle_0143FE30]

closed:
        mov byte ptr [esi + 14h], 0
        pop esi
        ret
    }
}
