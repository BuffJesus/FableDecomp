#include "fable_string.h"

// See the sibling 0x0099BF30 overload.  The helpers keep call relocations
// symbolic while the naked body fixes only the retail register allocation.
extern "C" void __fastcall
FableWideStringConstructLiteral_0099B6B0(
    CWideString* self,
    void*,
    const wchar_t* text);
extern "C" const CWideString& __fastcall
FableWideStringAppendWide_0099B8D0(
    CWideString* self,
    void*,
    const CWideString& text);
extern "C" void __fastcall
FableWideStringUnassign_0099B4D0(CWideString* self);
extern "C" void __cdecl FableWideStringFree_00BFEA14(void* memory);
extern "C" void __cdecl FableWideStringDelete_00BFE9BC(void* memory);

__declspec(naked) CWideString FABLE_FASTCALL operator+(
    const wchar_t* left,
    const CWideString& right)
{
    __asm
    {
        push ecx
        push esi
        push edi
        mov edi, ecx
        push edx
        lea ecx, [esp + 0Ch]
        call FableWideStringConstructLiteral_0099B6B0
        mov eax, dword ptr [esp + 10h]
        push eax
        lea ecx, [esp + 0Ch]
        call FableWideStringAppendWide_0099B8D0
        mov esi, dword ptr [esp + 8]
        mov dword ptr [edi], 0
        inc dword ptr [g_CWideStringInstanceCount_013BCA20]
        lea ecx, [esp + 8]
        cmp ecx, edi
        je result_copy_done
        mov eax, dword ptr [edi]
        cmp esi, eax
        je result_copy_done
        test eax, eax
        je result_unassigned
        mov ecx, edi
        call FableWideStringUnassign_0099B4D0

result_unassigned:
        test esi, esi
        je cleanup_done
        mov dword ptr [edi], esi
        inc dword ptr [esi + 0Ch]

result_copy_done:
        test esi, esi
        je cleanup_done
        mov ecx, dword ptr [esi + 0Ch]
        dec ecx
        mov eax, ecx
        test eax, eax
        mov dword ptr [esi + 0Ch], ecx
        jg cleanup_done
        mov eax, dword ptr [esi]
        test eax, eax
        je text_freed
        push eax
        call FableWideStringFree_00BFEA14
        add esp, 4

text_freed:
        push esi
        call FableWideStringDelete_00BFE9BC
        add esp, 4

cleanup_done:
        dec dword ptr [g_CWideStringInstanceCount_013BCA20]
        mov eax, edi
        pop edi
        pop esi
        pop ecx
        ret 4
    }
}
