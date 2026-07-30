#include "fable_string.h"

extern "C" void FABLE_FASTCALL
FableWideAddAssign_0099BE70(
    void* self,
    void*,
    const void* right);
extern "C" void FABLE_FASTCALL
FableWideUnassign_0099BE70(void* self, void*);
extern "C" void FABLE_CDECL
FableWideAddFree_0099BE70(void* memory);
extern "C" void FABLE_CDECL
FableWideAddDelete_0099BE70(void* memory);

// operator+(CWideString const&, CWideString const&) @ 0x0099BE70.
__declspec(naked) CWideString FABLE_FASTCALL operator+(
    const CWideString&,
    const CWideString&)
{
    __asm
    {
        push ecx
        push esi
        mov esi, dword ptr [g_CWideStringInstanceCount_013BCA20]
        inc esi
        lea eax, dword ptr [esp + 4]
        cmp edx, eax
        push edi
        mov edi, ecx
        mov dword ptr [esp + 8], 0
        mov dword ptr [g_CWideStringInstanceCount_013BCA20], esi
        je temporary_ready
        mov edx, dword ptr [edx]
        test edx, edx
        je temporary_ready
        mov eax, dword ptr [edx + 0Ch]
        inc eax
        mov dword ptr [esp + 8], edx
        mov dword ptr [edx + 0Ch], eax

    temporary_ready:
        mov ecx, dword ptr [esp + 10h]
        push ecx
        lea ecx, dword ptr [esp + 0Ch]
        call FableWideAddAssign_0099BE70
        mov esi, dword ptr [esp + 8]
        mov dword ptr [edi], 0
        mov ecx, dword ptr [g_CWideStringInstanceCount_013BCA20]
        inc ecx
        lea edx, dword ptr [esp + 8]
        cmp edx, edi
        mov dword ptr [g_CWideStringInstanceCount_013BCA20], ecx
        je destroy_temporary
        mov eax, dword ptr [edi]
        cmp esi, eax
        je destroy_temporary
        test eax, eax
        je assign_result
        mov ecx, edi
        call FableWideUnassign_0099BE70

    assign_result:
        test esi, esi
        je finished
        mov dword ptr [edi], esi
        inc dword ptr [esi + 0Ch]

    destroy_temporary:
        test esi, esi
        je finished
        mov ecx, dword ptr [esi + 0Ch]
        dec ecx
        mov eax, ecx
        test eax, eax
        mov dword ptr [esi + 0Ch], ecx
        jg finished
        mov eax, dword ptr [esi]
        test eax, eax
        je delete_temporary_data
        push eax
        call FableWideAddFree_0099BE70
        add esp, 4

    delete_temporary_data:
        push esi
        call FableWideAddDelete_0099BE70
        add esp, 4

    finished:
        dec dword ptr [g_CWideStringInstanceCount_013BCA20]
        mov eax, edi
        pop edi
        pop esi
        pop ecx
        ret 4
    }
}
