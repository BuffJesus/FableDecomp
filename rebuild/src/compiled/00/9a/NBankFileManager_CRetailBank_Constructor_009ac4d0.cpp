#include "fable_retail_banks.h"

extern "C" fable_u8 g_FableRetailBankVTable_0129B010;

extern "C" void FABLE_FASTCALL
FableRetailBankBaseConstruct_009AC4D0(void* bank, void*);
extern "C" void* FABLE_CDECL
FableRetailBankAllocateNode_009AC4D0(fable_u32 size);
extern "C" void FABLE_FASTCALL
FableRetailBankPathConstruct_009AC4D0(void* path, void*);

// NBankFileManager::CRetailBank::CRetailBank() @ 0x009AC4D0.
__declspec(naked) NBankFileManager::CRetailBank::CRetailBank()
{
    __asm
    {
        push ebx
        push esi
        mov esi, ecx
        call FableRetailBankBaseConstruct_009AC4D0
        xor ebx, ebx
        mov dword ptr [esi], offset g_FableRetailBankVTable_0129B010
        mov dword ptr [esi + 4], ebx
        mov dword ptr [esi + 8], ebx
        mov dword ptr [esi + 0Ch], ebx
        mov dword ptr [esi + 10h], ebx
        mov byte ptr [esi + 14h], bl
        push 28h
        mov dword ptr [esi + 18h], ebx
        call FableRetailBankAllocateNode_009AC4D0
        mov dword ptr [esi + 18h], eax
        mov dword ptr [esi + 1Ch], ebx
        mov byte ptr [eax], bl
        mov eax, dword ptr [esi + 18h]
        mov dword ptr [eax + 4], ebx
        mov eax, dword ptr [esi + 18h]
        mov dword ptr [eax + 8], eax
        mov eax, dword ptr [esi + 18h]
        add esp, 4
        lea ecx, dword ptr [esi + 24h]
        mov dword ptr [eax + 0Ch], eax
        call FableRetailBankPathConstruct_009AC4D0
        mov eax, esi
        pop esi
        pop ebx
        ret
    }
}
