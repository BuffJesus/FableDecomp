#include "fable_game.h"

namespace
{
    const wchar_t kLugSearchPattern[] = L"*.lug";
    const wchar_t kMetExtension[] = L".met";
}

extern "C" fable_u32 g_FableSecurityCookie_0139C8A8;
extern "C" fable_u8 g_FableMetPersistVTable_0122ED7C;

extern "C" void* __fastcall FableMetGetAutoSaveName_00418c3b(void*, void*);
extern "C" void* __fastcall FableMetAssignValue_00418c3b(void*, const void*, const void*);
extern "C" void __fastcall FableMetWideDtor_00418c3b(void*, void*);
extern "C" bool __fastcall FableMetFindFirst_00418c3b(void*, void*, void*);
extern "C" void __fastcall FableMetWideCtor_00418c3b(void*, void*);
extern "C" void __fastcall FableMetWideLiteralCtor_00418c3b(void*, void*, const wchar_t*);
extern "C" void __fastcall FableMetSplitPath_00418c3b(void*, void*, void*, void*, void*);
extern "C" void* __fastcall FableMetAssignData_00418c3b(
    void*,
    const void*,
    const void*);
extern "C" void __fastcall FableMetMemoryFileCtor_00418c3b(void*, void*, const void*, fable_u32, fable_u32);
extern "C" void* FableMetGetSystemManager_00418c3b();
extern "C" void __fastcall FableMetDumpToFile_00418c3b(void*, void*);
extern "C" bool __fastcall FableMetFindNext_00418c3b(void*, void*, void*);
extern "C" void __fastcall FableMetMemoryFileDtor_00418c3b(void*, void*);
extern "C" void __fastcall FableMetSecurityCheck_00418c3b(fable_u32, void*);

__declspec(naked) void FABLE_FASTCALL
CMainGameComponent::GenerateMetFilesFromLugFiles()
{
    __asm
    {
        push ebp
        lea ebp, dword ptr [esp - 1ECh]
        sub esp, 26Ch
        mov eax, dword ptr [g_FableSecurityCookie_0139C8A8]
        push offset kLugSearchPattern
        lea ecx, dword ptr [ebp - 2Ch]
        mov dword ptr [ebp + 1E8h], eax
        call FableMetGetAutoSaveName_00418c3b
        mov edx, eax
        lea ecx, dword ptr [ebp - 30h]
        call FableMetAssignValue_00418c3b
        lea ecx, dword ptr [ebp - 2Ch]
        call FableMetWideDtor_00418c3b
        lea eax, dword ptr [ebp - 24h]
        push eax
        lea edx, dword ptr [ebp - 5Ch]
        lea ecx, dword ptr [ebp - 30h]
        call FableMetFindFirst_00418c3b
        test al, al
        jz enumeration_done
        push ebx
        push esi
        push edi
        mov esi, offset g_FableMetPersistVTable_0122ED7C

    process_file:
        lea ecx, dword ptr [ebp - 28h]
        call FableMetWideCtor_00418c3b
        lea ecx, dword ptr [ebp - 34h]
        call FableMetWideCtor_00418c3b
        lea eax, dword ptr [ebp - 24h]
        push eax
        lea ecx, dword ptr [ebp - 2Ch]
        call FableMetWideLiteralCtor_00418c3b
        lea eax, dword ptr [ebp - 34h]
        push eax
        lea eax, dword ptr [ebp - 28h]
        push eax
        push 0
        xor edx, edx
        lea ecx, dword ptr [ebp - 2Ch]
        call FableMetSplitPath_00418c3b
        lea ecx, dword ptr [ebp - 2Ch]
        call FableMetWideDtor_00418c3b
        push 2
        push 3
        push offset kMetExtension
        lea eax, dword ptr [ebp - 28h]
        push eax
        lea ecx, dword ptr [ebp - 38h]
        call FableMetGetAutoSaveName_00418c3b
        mov edx, eax
        lea ecx, dword ptr [ebp - 50h]
        call FableMetAssignData_00418c3b
        mov edx, eax
        lea ecx, dword ptr [ebp - 54h]
        call FableMetAssignValue_00418c3b
        push eax
        lea ecx, dword ptr [ebp - 80h]
        call FableMetMemoryFileCtor_00418c3b
        lea ecx, dword ptr [ebp - 54h]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 50h]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 38h]
        call FableMetWideDtor_00418c3b
        lea eax, dword ptr [ebp - 80h]
        mov dword ptr [ebp - 44h], esi
        mov dword ptr [ebp - 40h], eax
        call FableMetGetSystemManager_00418c3b
        mov edi, dword ptr [eax + 7Ch]
        mov ebx, dword ptr [edi]
        lea eax, dword ptr [ebp - 44h]
        push eax
        lea eax, dword ptr [ebp - 34h]
        push eax
        lea eax, dword ptr [ebp - 28h]
        push eax
        lea ecx, dword ptr [ebp - 3Ch]
        call FableMetGetAutoSaveName_00418c3b
        mov edx, eax
        lea ecx, dword ptr [ebp - 4Ch]
        call FableMetAssignData_00418c3b
        mov edx, eax
        lea ecx, dword ptr [ebp - 48h]
        call FableMetAssignData_00418c3b
        push eax
        mov ecx, edi
        call dword ptr [ebx + 0CCh]
        lea ecx, dword ptr [ebp - 48h]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 4Ch]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 3Ch]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 80h]
        call FableMetDumpToFile_00418c3b
        lea eax, dword ptr [ebp - 24h]
        push eax
        lea edx, dword ptr [ebp - 5Ch]
        lea ecx, dword ptr [ebp - 30h]
        call FableMetFindNext_00418c3b
        lea ecx, dword ptr [ebp - 80h]
        mov bl, al
        mov dword ptr [ebp - 44h], esi
        call FableMetMemoryFileDtor_00418c3b
        lea ecx, dword ptr [ebp - 34h]
        call FableMetWideDtor_00418c3b
        lea ecx, dword ptr [ebp - 28h]
        call FableMetWideDtor_00418c3b
        test bl, bl
        jnz process_file
        pop edi
        pop esi
        pop ebx

    enumeration_done:
        lea ecx, dword ptr [ebp - 30h]
        call FableMetWideDtor_00418c3b
        mov ecx, dword ptr [ebp + 1E8h]
        call FableMetSecurityCheck_00418c3b
        add ebp, 1ECh
        leave
        ret
    }
}
