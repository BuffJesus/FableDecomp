#include "fable_window_title.h"

namespace
{
    const wchar_t kFableWindowTitle[] = L"Fable - The Lost Chapters";
    const char kFableWindowTitleTag[] = "TEXT_GUI_WINDOW_TITLE";
}

extern "C" fable_u8 g_FableEmptyCharString_0129AAF4;
extern "C" fable_u8 g_FableWindowTitleValue_0122D820;

extern "C" void __fastcall FableWindowTitleWideLiteralCtor_004023f0(
    void*,
    void*,
    const wchar_t*);
extern "C" void __fastcall FableWindowTitleCharLiteralCtor_004023f0(
    void*,
    void*,
    const char*,
    long);
extern "C" void* __cdecl FableWindowTitleOperatorNew_004023f0(fable_u32);
extern "C" void* __fastcall FableWindowTitleBankCtor_004023f0(
    void*,
    void*);
extern "C" void* __fastcall FableWindowTitleGetFontBankName_004023f0(
    void*,
    void*);
extern "C" void __fastcall FableWindowTitleCharDtor_004023f0(
    void*,
    void*);
extern "C" fable_u32 __fastcall FableWindowTitleCrc_004023f0(
    fable_u32,
    const void*,
    fable_u32);
extern "C" fable_u32 __fastcall FableWindowTitleLookup_004023f0(
    void*,
    void*,
    fable_u32);
extern "C" void __fastcall FableWindowTitleGetGroupEntry_004023f0(
    void*,
    void*,
    void*,
    fable_u32,
    bool);
extern "C" void* __fastcall FableWindowTitleGetBankHandle_004023f0(
    void*,
    void*,
    void*);
extern "C" void* __fastcall FableWindowTitleWideAssign_004023f0(
    void*,
    void*,
    const void*);
extern "C" void __fastcall FableWindowTitleWideDtor_004023f0(
    void*,
    void*);
extern "C" void* __fastcall FableWindowTitleAssignResult_004023f0(
    void*,
    const void*,
    const void*);

__declspec(naked) CWideString FABLE_FASTCALL GetWindowTitle()
{
    __asm
    {
        sub esp, 10h
        push ebx
        push esi
        push edi
        mov edi, ecx
        push offset kFableWindowTitle
        lea ecx, dword ptr [esp + 10h]
        call FableWindowTitleWideLiteralCtor_004023f0
        push -1
        push offset kFableWindowTitleTag
        lea ecx, dword ptr [esp + 1Ch]
        call FableWindowTitleCharLiteralCtor_004023f0
        push 214h
        call FableWindowTitleOperatorNew_004023f0
        add esp, 4
        test eax, eax
        jz bank_allocation_failed
        mov ecx, eax
        call FableWindowTitleBankCtor_004023f0
        mov esi, eax
        jmp bank_ready

    bank_allocation_failed:
        xor esi, esi

    bank_ready:
        mov ebx, dword ptr [esi]
        push 100h
        lea ecx, dword ptr [esp + 14h]
        call FableWindowTitleGetFontBankName_004023f0
        push eax
        mov ecx, esi
        call dword ptr [ebx + 4]
        lea ecx, dword ptr [esp + 10h]
        call FableWindowTitleCharDtor_004023f0
        mov ecx, dword ptr [esp + 14h]
        test ecx, ecx
        jnz tag_has_storage
        xor eax, eax
        mov edx, offset g_FableEmptyCharString_0129AAF4
        jmp calculate_crc

    tag_has_storage:
        mov eax, dword ptr [ecx + 4]
        mov edx, dword ptr [ecx]

    calculate_crc:
        push eax
        xor ecx, ecx
        call FableWindowTitleCrc_004023f0
        push eax
        mov ecx, esi
        call FableWindowTitleLookup_004023f0
        test eax, eax
        jbe assign_result
        push 1
        push eax
        lea eax, dword ptr [esp + 18h]
        push eax
        mov ecx, esi
        call FableWindowTitleGetGroupEntry_004023f0
        mov ecx, dword ptr [esp + 10h]
        test ecx, ecx
        jz assign_result
        lea edx, dword ptr [esp + 18h]
        push edx
        call FableWindowTitleGetBankHandle_004023f0
        push eax
        lea ecx, dword ptr [esp + 10h]
        call FableWindowTitleWideAssign_004023f0
        lea ecx, dword ptr [esp + 18h]
        call FableWindowTitleWideDtor_004023f0
        mov eax, dword ptr [esp + 10h]
        test eax, eax
        jz assign_result
        mov ebx, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec ebx
        mov edx, ebx
        test edx, edx
        mov dword ptr [eax], ebx
        jnz assign_result
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 4]

    assign_result:
        push offset g_FableWindowTitleValue_0122D820
        lea edx, dword ptr [esp + 10h]
        mov ecx, edi
        call FableWindowTitleAssignResult_004023f0
        lea ecx, dword ptr [esp + 14h]
        call FableWindowTitleCharDtor_004023f0
        mov edx, dword ptr [esi]
        push 1
        mov ecx, esi
        call dword ptr [edx]
        lea ecx, dword ptr [esp + 0Ch]
        call FableWindowTitleWideDtor_004023f0
        mov eax, edi
        pop edi
        pop esi
        pop ebx
        add esp, 10h
        ret
    }
}
