#include "rebuild_abi.h"

#include <stddef.h>

struct FableWideString_00999230
{
    void* storage;
};

extern "C" void FABLE_FASTCALL
FableWideStringConstruct_0099AED0(
    FableWideString_00999230* self,
    void*);
extern "C" FableWideString_00999230* FABLE_FASTCALL
FableGetCurrentPath_00997F80(
    FableWideString_00999230* result,
    void*);
extern "C" FableWideString_00999230* FABLE_FASTCALL
FableExpandPath_00998C90(
    FableWideString_00999230* result,
    const FableWideString_00999230* pathname,
    const FableWideString_00999230* context);
extern "C" FableWideString_00999230* FABLE_FASTCALL
FableWideStringAssign_0099B7D0(
    FableWideString_00999230* self,
    void*,
    const FableWideString_00999230* other);
extern "C" void FABLE_FASTCALL
FableWideStringDestroy_0099B510(
    FableWideString_00999230* self,
    void*);
extern "C" const wchar_t* FABLE_FASTCALL
FableWideStringGetBuffer_0099AF00(
    FableWideString_00999230* self,
    void*);

typedef unsigned long (__stdcall *FableGetFileAttributesWProc)(
    const wchar_t*);
extern "C" FableGetFileAttributesWProc
g_FableGetFileAttributesW_0143FDBC;

// CAFile::PathExists(CWideString const&) @ 0x00999230.
//
// Retail builds an expanded, process-relative pathname and regards every
// GetFileAttributesW value except INVALID_FILE_ATTRIBUTES as an existing
// path.  The explicit body preserves the VC7.1 temporary lifetime and cleanup
// order.
extern "C" __declspec(naked) bool FABLE_FASTCALL
FablePathExists_00999230(
    const FableWideString_00999230&,
    void*)
{
    __asm
    {
        sub esp, 0Ch
        push esi
        mov esi, ecx
        lea ecx, [esp + 4]
        call FableWideStringConstruct_0099AED0
        lea ecx, [esp + 0Ch]
        call FableGetCurrentPath_00997F80
        push eax
        mov edx, esi
        lea ecx, [esp + 0Ch]
        call FableExpandPath_00998C90
        push eax
        lea ecx, [esp + 8]
        call FableWideStringAssign_0099B7D0
        lea ecx, [esp + 8]
        call FableWideStringDestroy_0099B510
        lea ecx, [esp + 0Ch]
        call FableWideStringDestroy_0099B510
        lea ecx, [esp + 4]
        call FableWideStringGetBuffer_0099AF00
        push eax
        call dword ptr [g_FableGetFileAttributesW_0143FDBC]
        cmp eax, -1
        pop esi
        lea ecx, [esp]
        je missing
        call FableWideStringDestroy_0099B510
        mov al, 1
        add esp, 0Ch
        ret

missing:
        call FableWideStringDestroy_0099B510
        xor al, al
        add esp, 0Ch
        ret
    }
}
