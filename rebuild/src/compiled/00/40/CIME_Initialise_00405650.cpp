#include "fable_ime.h"

extern "C" void* g_FableImeDrawRectCallback_0143CD64;
extern "C" void* g_FableImeDrawFansCallback_0143E514;
extern "C" void* g_FableImeOnCharCallback_0143CF08;
extern "C" void* g_FableImeRenderCallback_0143CDFC;
extern "C" void* g_FableImeGetCaretCallback_0143E630;
extern "C" fable_u8 g_FableImeFontVTable_0122DCBC;

extern "C" void FableImeDrawRectCallback_00404D50();
extern "C" void FableImeDrawFansCallback_00404A60();
extern "C" void FableImeOnCharCallback_00404A70();
extern "C" void FableImeRenderCallback_00404E10();
extern "C" void FableImeGetCaretCallback_00404F00();

extern "C" void __fastcall FableImeCleanup_00405650(void*, void*);
extern "C" bool __fastcall FableImeUiInitialise_00405650(void*, fable_u8);
extern "C" void* __cdecl FableImeOperatorNew_00405650(fable_u32);
extern "C" void __fastcall FableImeSetAppearance_00405650(void*, void*);
extern "C" void* FableImeGetSystemManager_00405650();
extern "C" void __fastcall FableImeGetDrawContext_00405650(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableImeDraw_00405650(void*, void*);
extern "C" void __fastcall FableImeResetComposition_00405650(fable_u8, void*);
extern "C" void __fastcall FableImeEnable_00405650(fable_u8, void*);

__declspec(naked) void NUISystem::CIME::Initialise(HWND__*)
{
    __asm
    {
        sub esp, 8
        push esi
        mov esi, ecx
        call FableImeCleanup_00405650
        cmp byte ptr [esi], 0
        mov ecx, dword ptr [esp + 10h]
        jz initialise_disabled_ime
        mov dword ptr [g_FableImeDrawRectCallback_0143CD64], offset FableImeDrawRectCallback_00404D50
        mov dword ptr [g_FableImeDrawFansCallback_0143E514], offset FableImeDrawFansCallback_00404A60
        mov dword ptr [g_FableImeOnCharCallback_0143CF08], offset FableImeOnCharCallback_00404A70
        mov dword ptr [g_FableImeRenderCallback_0143CDFC], offset FableImeRenderCallback_00404E10
        mov dword ptr [g_FableImeGetCaretCallback_0143E630], offset FableImeGetCaretCallback_00404F00
        xor dl, dl
        mov dword ptr [esi + 4], ecx
        call FableImeUiInitialise_00405650
        test al, al
        mov byte ptr [esi + 8], al
        jz check_initialised
        push 14h
        call FableImeOperatorNew_00405650
        add esp, 4
        test eax, eax
        jz font_allocation_failed
        mov dword ptr [eax], offset g_FableImeFontVTable_0122DCBC
        jmp font_ready

    font_allocation_failed:
        xor eax, eax

    font_ready:
        lea ecx, dword ptr [esi + 0Ch]
        mov dword ptr [esi + 1Ch], eax
        call FableImeSetAppearance_00405650
        call FableImeGetSystemManager_00405650
        lea ecx, dword ptr [esp + 4]
        push ecx
        mov ecx, dword ptr [eax + 60h]
        call FableImeGetDrawContext_00405650
        mov al, byte ptr [esi + 8]
        test al, al
        jz initialise_done
        mov edx, dword ptr [esp + 8]
        mov ecx, dword ptr [esp + 4]
        call FableImeDraw_00405650

    check_initialised:
        mov al, byte ptr [esi + 8]
        test al, al
        jz initialise_done
        xor cl, cl
        call FableImeResetComposition_00405650
        xor cl, cl
        call FableImeEnable_00405650
        pop esi
        add esp, 8
        ret 4

    initialise_disabled_ime:
        mov dl, 1
        call FableImeUiInitialise_00405650

    initialise_done:
        pop esi
        add esp, 8
        ret 4
    }
}
