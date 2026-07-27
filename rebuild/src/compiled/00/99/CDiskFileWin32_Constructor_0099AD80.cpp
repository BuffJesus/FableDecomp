#include "rebuild_abi.h"

#include <stddef.h>

struct FableWideString_0099AD80
{
    void* storage;
};

#pragma pack(push, 1)
struct FableDiskFileWin32_0099AD80
{
    void** vtable;
    fable_u32 accessType;
    fable_i32 position;
    fable_i32 length;
    void* winFileHandle;
    fable_u8 opened;
    fable_u8 padding15[3];
    FableWideString_0099AD80 pathname;
};
#pragma pack(pop)

extern "C" void FABLE_FASTCALL
FableCAFileConstruct_0099A2F0(
    FableDiskFileWin32_0099AD80* self,
    void*);
extern "C" void FABLE_FASTCALL
FableWideStringConstructForDisk_0099AED0(
    FableWideString_0099AD80* self,
    void*);
extern "C" void FABLE_FASTCALL
FableDiskFileOpen_0099A6A0(
    FableDiskFileWin32_0099AD80* self,
    void*,
    const FableWideString_0099AD80* pathname,
    fable_u32 accessType,
    fable_u32 flags);
extern "C" void* g_CDiskFileWin32VTable_0122D06C[];

FABLE_STATIC_ASSERT(sizeof(FableDiskFileWin32_0099AD80) == 0x1C);
FABLE_STATIC_ASSERT(offsetof(FableDiskFileWin32_0099AD80, pathname) == 0x18);

// CDiskFileWin32::CDiskFileWin32(CWideString const&, FileAccessType, ulong)
// @ 0x0099AD80.
extern "C" __declspec(naked) FableDiskFileWin32_0099AD80* FABLE_FASTCALL
FableConstructDiskFileWin32_0099AD80(
    FableDiskFileWin32_0099AD80*,
    void*,
    const FableWideString_0099AD80*,
    fable_u32,
    fable_u32)
{
    __asm
    {
        push esi
        mov esi, ecx
        call FableCAFileConstruct_0099A2F0
        lea ecx, [esi + 18h]
        mov dword ptr [esi], offset g_CDiskFileWin32VTable_0122D06C
        call FableWideStringConstructForDisk_0099AED0
        mov eax, dword ptr [esp + 10h]
        mov ecx, dword ptr [esp + 0Ch]
        mov edx, dword ptr [esp + 8]
        push eax
        push ecx
        push edx
        mov ecx, esi
        call FableDiskFileOpen_0099A6A0
        mov eax, esi
        pop esi
        ret 0Ch
    }
}
