#include "fable_game_text_bank.h"

extern "C" fable_u8 g_FableGameTextDataBankVTable_0129B4D4;
extern "C" fable_u8 g_FableGameTextResourceBankVTable_0129B528;

extern "C" void FABLE_FASTCALL
FableGameTextBankFileConstruct_009C85A0(void* bank, void*);
extern "C" void FABLE_FASTCALL
FableGameTextResourceBankConstruct_009C85A0(void* bank, void*);
extern "C" void FABLE_FASTCALL
FableGameTextSetMaximumMemory_009C85A0(
    void* resourceList,
    void*,
    fable_u32 maximumMemory);

// NGameText::CDataBank::CDataBank() @ 0x009C85A0.
__declspec(naked) NGameText::CDataBank::CDataBank()
{
    __asm
    {
        push esi
        push edi
        mov esi, ecx
        call FableGameTextBankFileConstruct_009C85A0
        lea edi, dword ptr [esi + 164h]
        mov ecx, edi
        call FableGameTextResourceBankConstruct_009C85A0
        mov dword ptr [edi], offset g_FableGameTextResourceBankVTable_0129B528
        xor edi, edi
        mov dword ptr [esi], offset g_FableGameTextDataBankVTable_0129B4D4
        mov dword ptr [esi + 1E0h], edi
        mov dword ptr [esi + 1E4h], edi
        mov dword ptr [esi + 1E8h], edi
        mov dword ptr [esi + 1ECh], edi
        mov dword ptr [esi + 1F0h], edi
        mov dword ptr [esi + 1F4h], edi
        mov dword ptr [esi + 1F8h], edi
        mov dword ptr [esi + 1FCh], edi
        mov dword ptr [esi + 200h], edi
        mov dword ptr [esi + 204h], edi
        mov dword ptr [esi + 208h], edi
        mov dword ptr [esi + 20Ch], edi
        push 10000h
        lea ecx, dword ptr [esi + 168h]
        mov dword ptr [esi + 210h], edi
        call FableGameTextSetMaximumMemory_009C85A0
        mov dword ptr [esi + 19Ch], edi
        pop edi
        mov eax, esi
        pop esi
        ret
    }
}
