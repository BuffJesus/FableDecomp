#include "rebuild_abi.h"

struct FableCharString_0099F2E0
{
    void* storage;
};

extern "C" void FABLE_FASTCALL
FableCharStringRelease_0099E9B0(
    FableCharString_0099F2E0* self,
    void*);
extern "C" void* FABLE_CDECL
FableCharStringAllocate_00BFEA1A(unsigned long size);
extern "C" void FABLE_FASTCALL
FableCharStringStorageInitialise_009A0390(
    void* storage,
    void*,
    long length,
    unsigned long growth);

// CCharString::LoadFromFile(
//     CAFile&,
//     CASuspendableProcess*,
//     long) @ 0x0099F2E0.
//
// A length of -1 means "from the current file position to EOF". Retail
// replaces any prior string allocation, creates a 17-byte shared string
// descriptor, initializes its buffer with growth 0x20, then requests exactly
// the selected byte count from CAFile.
extern "C" __declspec(naked) void FABLE_FASTCALL
FableCharStringLoadFromFile_0099F2E0(
    FableCharString_0099F2E0* self,
    void*,
    void* file,
    void* suspendableProcess,
    long length)
{
    __asm
    {
        push ecx
        push ebx
        mov ebx, ecx
        cmp dword ptr [ebx], 0
        push ebp
        push esi
        push edi
        mov dword ptr [esp + 10h], ebx
        je noOldString
        call FableCharStringRelease_0099E9B0

noOldString:
        mov edi, dword ptr [esp + 20h]
        cmp edi, -1
        mov ebp, dword ptr [esp + 18h]
        jne haveLength
        mov eax, dword ptr [ebp]
        mov ecx, ebp
        call dword ptr [eax + 24h]
        mov edx, dword ptr [ebp]
        mov ecx, ebp
        mov edi, eax
        call dword ptr [edx + 1Ch]
        sub edi, eax

haveLength:
        push 11h
        call FableCharStringAllocate_00BFEA1A
        mov esi, eax
        add esp, 4
        test esi, esi
        je allocationFailed
        mov ebx, dword ptr [esi + 8]
        mov dl, byte ptr [esi + 0Ch]
        and ebx, 80000000h
        mov dword ptr [esi + 8], ebx
        mov ebx, 1
        push 20h
        or dl, bl
        push edi
        mov ecx, esi
        mov dword ptr [esi], 0
        mov dword ptr [esi + 4], 0
        mov byte ptr [esi + 0Ch], dl
        call FableCharStringStorageInitialise_009A0390
        mov dword ptr [esi + 0Dh], ebx
        mov ebx, dword ptr [esp + 10h]
        jmp publishStorage

allocationFailed:
        xor esi, esi

publishStorage:
        test esi, esi
        mov dword ptr [ebx], esi
        je done
        test edi, edi
        jle done
        mov ecx, dword ptr [esp + 1Ch]
        mov edx, dword ptr [esi]
        mov eax, dword ptr [ebp]
        push ecx
        push edi
        push edx
        mov ecx, ebp
        call dword ptr [eax + 0Ch]

done:
        pop edi
        pop esi
        pop ebp
        pop ebx
        pop ecx
        ret 0Ch
    }
}
