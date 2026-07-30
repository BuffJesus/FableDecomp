#include "fable_system.h"

#include <string.h>

namespace
{
    const fable_u32 kDefaultDisplayWidth = 128;
    const fable_u32 kDefaultDisplayHeight = 64;
    const fable_u32 kDefaultMemoryBudget = 0x00040000UL;
    const fable_u32 kRegionAllocationSize = 0x00020000UL;
    const char kRegionGuid[] = "BBB GUID";
    const wchar_t kPrimaryIdentifier[] = L"b";
    const wchar_t kSecondaryIdentifier[] = L"C";
}

// The retail constructor interleaves scalar initialisation with four non-trivial
// member constructions. Keep that VC7.1 schedule explicit so the recovered
// layout and all twelve external references remain independently relocatable.
__declspec(naked) CSystemManagerInit::CSystemManagerInit()
{
    __asm
    {
        push ebx
        push ebp
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esi + 0ch]
        call FableConstructWideString
        lea ecx, [esi + 02ch]
        call FableConstructCharString
        lea ecx, [esi + 030h]
        call FableConstructCharString
        xor ebx, ebx
        mov dword ptr [esi + 03ch], 040h
        mov dword ptr [esi + 038h], 080h
        mov dword ptr [esi + 040h], 042800000h
        mov dword ptr [esi + 044h], 040000000h
        mov dword ptr [esi + 048h], 03f800000h
        mov dword ptr [esi + 050h], ebx
        mov dword ptr [esi + 04ch], ebx
        mov dword ptr [esi + 054h], 000040000h
        lea ecx, [esi + 05ch]
        mov byte ptr [esi + 058h], bl
        mov byte ptr [esi + 059h], bl
        mov byte ptr [esi + 05ah], bl
        call CRegionDef::ConstructStorage
        lea edi, [esi + 0b8h]
        mov ecx, edi
        call FableConstructWideString
        lea ebp, [esi + 0bch]
        mov ecx, ebp
        call FableConstructWideString
        mov dword ptr [esi], ebx
        mov dword ptr [esi + 04h], ebx
        mov byte ptr [esi + 010h], 1
        mov dword ptr [esi + 014h], ebx
        mov dword ptr [esi + 09ch], ebx
        mov byte ptr [esi + 011h], bl
        lea eax, [esi + 0a0h]
        xor ecx, ecx
        mov edx, eax
        mov dword ptr [edx], ecx
        mov dword ptr [edx + 04h], ecx
        mov dword ptr [edx + 08h], ecx
        push 8
        mov dword ptr [edx + 0ch], ecx
        push offset kRegionGuid
        push eax
        mov byte ptr [edx + 010h], cl
        call strncpy
        add esp, 0ch
        push offset kPrimaryIdentifier
        mov ecx, edi
        mov byte ptr [esi + 0a8h], bl
        call FableAssignWideString
        push offset kSecondaryIdentifier
        mov ecx, ebp
        mov dword ptr [esi + 0b4h], 000020000h
        mov byte ptr [esi + 019h], bl
        mov byte ptr [esi + 020h], bl
        mov dword ptr [esi + 01ch], ebx
        mov byte ptr [esi + 021h], bl
        mov dword ptr [esi + 024h], ebx
        mov dword ptr [esi + 028h], ebx
        call FableAssignWideString
        pop edi
        mov dword ptr [esi + 0c0h], ebx
        mov dword ptr [esi + 0c4h], ebx
        mov byte ptr [esi + 034h], 1
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        ret
    }
}
