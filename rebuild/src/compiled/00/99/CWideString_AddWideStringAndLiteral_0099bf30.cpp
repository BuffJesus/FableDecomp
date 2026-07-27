#include "fable_string.h"

// Retail inlines the copy-constructor and destructor around operator+=.
// VC7.1 reproduces the semantics from ordinary C++, but chooses a different
// register allocation in this translation unit.  Preserve the recovered
// ownership algorithm and spell the retail allocation explicitly.
extern "C" const CWideString& __fastcall
FableWideStringAppendLiteral_0099B940(
    CWideString* self,
    void*,
    const wchar_t* text);
extern "C" void __fastcall
FableWideStringUnassign_0099B4D0(CWideString* self);
extern "C" void __cdecl FableWideStringFree_00BFEA14(void* memory);
extern "C" void __cdecl FableWideStringDelete_00BFE9BC(void* memory);

__declspec(naked) CWideString FABLE_FASTCALL operator+(
    const CWideString& left,
    const wchar_t* right)
{
    __asm
    {
        push ecx
        push esi
        mov esi, dword ptr [g_CWideStringInstanceCount_013BCA20]
        inc esi
        lea eax, [esp + 4]
        cmp edx, eax
        push edi
        mov edi, ecx
        mov dword ptr [esp + 8], 0
        mov dword ptr [g_CWideStringInstanceCount_013BCA20], esi
        je first_copy_done
        mov edx, dword ptr [edx]
        test edx, edx
        je first_copy_done
        mov eax, dword ptr [edx + 0Ch]
        inc eax
        mov dword ptr [esp + 8], edx
        mov dword ptr [edx + 0Ch], eax

first_copy_done:
        mov ecx, dword ptr [esp + 10h]
        push ecx
        lea ecx, [esp + 0Ch]
        call FableWideStringAppendLiteral_0099B940
        mov esi, dword ptr [esp + 8]
        mov dword ptr [edi], 0
        mov ecx, dword ptr [g_CWideStringInstanceCount_013BCA20]
        inc ecx
        lea edx, [esp + 8]
        cmp edx, edi
        mov dword ptr [g_CWideStringInstanceCount_013BCA20], ecx
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
