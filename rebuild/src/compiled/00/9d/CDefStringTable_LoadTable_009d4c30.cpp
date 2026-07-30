#include "fable_definition_table.h"

extern "C" fable_u8 g_FableDiskFileVTable_0122D06C;

extern "C" bool FABLE_FASTCALL
FableDefTablePathExists_009D4C30(void* pathname, void*);
extern "C" void FABLE_FASTCALL
FableDefTableFileBaseConstruct_009D4C30(void* file, void*);
extern "C" void FABLE_FASTCALL
FableDefTableCharStringConstruct_009D4C30(void* string, void*);
extern "C" void FABLE_FASTCALL
FableDefTableDiskFileOpen_009D4C30(
    void* file,
    void*,
    const void* pathname,
    fable_u32 access,
    fable_u32 flags);
extern "C" void FABLE_FASTCALL
FableDefTableInputStreamConstruct_009D4C30(
    void* stream,
    void*,
    void* file,
    fable_u32 bufferSize);
extern "C" void FABLE_FASTCALL
FableDefTableRead_009D4C30(
    void* table,
    void*,
    void* stream);
extern "C" void FABLE_FASTCALL
FableDefTableInputStreamDestroy_009D4C30(void* stream, void*);
extern "C" void FABLE_FASTCALL
FableDefTableDiskFileClose_009D4C30(void* file, void*);
extern "C" void FABLE_FASTCALL
FableDefTableCharStringDestroy_009D4C30(void* string, void*);
extern "C" void FABLE_FASTCALL
FableDefTableFileBaseDestroy_009D4C30(void* file, void*);

// CDefStringTable::LoadTable(bool) @ 0x009D4C30.
__declspec(naked) void CDefStringTable::LoadTable(bool)
{
    __asm
    {
        sub esp, 40h
        push esi
        mov esi, ecx
        push edi
        lea edi, dword ptr [esi + 30h]
        mov ecx, edi
        call FableDefTablePathExists_009D4C30
        test al, al
        jz finished
        lea ecx, dword ptr [esp + 8]
        call FableDefTableFileBaseConstruct_009D4C30
        lea ecx, dword ptr [esp + 20h]
        mov dword ptr [esp + 8], offset g_FableDiskFileVTable_0122D06C
        call FableDefTableCharStringConstruct_009D4C30
        push 4
        push 1
        push edi
        lea ecx, dword ptr [esp + 14h]
        mov byte ptr [esp + 28h], 0
        call FableDefTableDiskFileOpen_009D4C30
        push 4000h
        lea eax, dword ptr [esp + 0Ch]
        push eax
        lea ecx, dword ptr [esp + 2Ch]
        call FableDefTableInputStreamConstruct_009D4C30
        lea ecx, dword ptr [esp + 24h]
        push ecx
        mov ecx, esi
        call FableDefTableRead_009D4C30
        mov dl, byte ptr [esp + 4Ch]
        lea ecx, dword ptr [esp + 24h]
        mov byte ptr [esi + 34h], 1
        mov byte ptr [esi + 35h], dl
        call FableDefTableInputStreamDestroy_009D4C30
        lea ecx, dword ptr [esp + 8]
        mov dword ptr [esp + 8], offset g_FableDiskFileVTable_0122D06C
        call FableDefTableDiskFileClose_009D4C30
        lea ecx, dword ptr [esp + 20h]
        call FableDefTableCharStringDestroy_009D4C30
        lea ecx, dword ptr [esp + 8]
        call FableDefTableFileBaseDestroy_009D4C30

    finished:
        pop edi
        pop esi
        add esp, 40h
        ret 4
    }
}
