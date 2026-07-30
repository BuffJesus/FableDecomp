#include "fable_game_text_bank.h"

extern "C" fable_u32 FABLE_FASTCALL
FableGameTextGetEntryDataType_009C9070(
    const void* bank,
    void*,
    fable_u32 index);
extern "C" void FABLE_FASTCALL
FableGameTextPrepareEntry_009C9070(void* bank, void*);
extern "C" bool FABLE_FASTCALL
FableGameTextAsyncIsFinished_009C9070(void* asyncData, void*);
extern "C" void FABLE_FASTCALL
FableGameTextBeginLoadingText_009C9070(
    void* bank,
    void*,
    fable_u32 index);
extern "C" void FABLE_FASTCALL
FableGameTextGroupBeginLoading_009C9070(
    void* groupEntry,
    void*,
    fable_u32 index,
    void* bank);
extern "C" void FABLE_FASTCALL
FableGameTextBuildGroup_009C9070(void* groupEntry, void*);
extern "C" void FABLE_FASTCALL
FableGameTextMakeFreeSpace_009C9070(
    void* resourceList,
    void*,
    fable_u32 force);
extern "C" void FABLE_FASTCALL
FableGameTextResourceListRemove_009C9070(void* resource, void*);
extern "C" void FABLE_FASTCALL
FableGameTextResourceListInsert_009C9070(
    void* resource,
    void*,
    void* nextResource,
    void* headResource);

// NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const
// @ 0x009C9070.
__declspec(naked) CIVCountedPointer<const NGameText::CGroupEntry>
NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const
{
    __asm
    {
        push ebx
        mov ebx, dword ptr [esp + 0Ch]
        push esi
        push edi
        push ebx
        mov esi, ecx
        call FableGameTextGetEntryDataType_009C9070
        test eax, eax
        jnz output_null
        mov eax, dword ptr [esi + 1E0h]
        mov edi, dword ptr [eax + ebx * 4]
        test edi, edi
        jz output_null
        mov ecx, esi
        call FableGameTextPrepareEntry_009C9070
        mov eax, dword ptr [edi]
        test eax, eax
        jz check_pending
        mov eax, dword ptr [eax + 38h]
        test eax, eax
        jz finish_loading
        mov ecx, eax
        call FableGameTextAsyncIsFinished_009C9070
        test al, al
        jnz finish_loading

    check_pending:
        mov al, byte ptr [esp + 18h]
        test al, al
        jnz finish_loading
        push ebx
        mov ecx, esi
        call FableGameTextBeginLoadingText_009C9070

    output_null:
        mov eax, dword ptr [esp + 10h]
        mov dword ptr [eax], 0

    cleanup:
        pop edi
        pop esi
        pop ebx
        ret 0Ch

    finish_loading:
        mov eax, dword ptr [edi]
        test eax, eax
        jnz maybe_build_existing
        push ebx
        mov ecx, esi
        call FableGameTextGetEntryDataType_009C9070
        test eax, eax
        jnz build_new
        mov ecx, dword ptr [esi + 1E0h]
        mov ecx, dword ptr [ecx + ebx * 4]
        test ecx, ecx
        jz build_new
        push esi
        push ebx
        call FableGameTextGroupBeginLoading_009C9070

    build_new:
        mov ecx, edi
        call FableGameTextBuildGroup_009C9070
        push 0
        lea ecx, dword ptr [esi + 168h]
        call FableGameTextMakeFreeSpace_009C9070
        jmp refresh_resource

    maybe_build_existing:
        mov ecx, dword ptr [eax + 38h]
        test ecx, ecx
        jz refresh_resource
        mov ecx, edi
        call FableGameTextBuildGroup_009C9070

    refresh_resource:
        mov esi, dword ptr [edi]
        mov eax, dword ptr [esi + 8]
        test eax, eax
        jz return_resource
        mov edx, dword ptr [esi + 18h]
        cmp edx, dword ptr [eax + 2Ch]
        jz return_resource
        mov ecx, esi
        call FableGameTextResourceListRemove_009C9070
        mov eax, dword ptr [esi + 8]
        mov edx, dword ptr [eax + 10h]
        lea ecx, dword ptr [eax + 4]
        push ecx
        push edx
        mov ecx, esi
        call FableGameTextResourceListInsert_009C9070
        mov eax, dword ptr [esi + 8]
        mov ecx, dword ptr [eax + 2Ch]
        mov dword ptr [esi + 18h], ecx

    return_resource:
        mov edi, dword ptr [edi]
        test edi, edi
        mov eax, dword ptr [esp + 10h]
        mov dword ptr [eax], edi
        jz cleanup
        inc dword ptr [edi + 4]
        pop edi
        pop esi
        pop ebx
        ret 0Ch
    }
}
