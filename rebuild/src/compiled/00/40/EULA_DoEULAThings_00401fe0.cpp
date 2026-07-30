#include "fable_eula.h"

namespace
{
    const unsigned short kEulaLibraryName[] = L"eula.dll";
    const char kEulaExportName[] = "EBUEula";
}

extern "C" void* (FABLE_STDCALL* g_FableEulaLoadLibraryW_00401fe0)(
    const unsigned short*);
extern "C" void* (FABLE_STDCALL* g_FableEulaGetProcAddress_00401fe0)(
    void*,
    const char*);
extern "C" int (FABLE_STDCALL* g_FableEulaFreeLibrary_00401fe0)(void*);

extern "C" fable_u8 g_FableEulaLanguage_013BCA28;
extern "C" fable_u8 g_FableEulaEmptyText_0129AAF4;
extern "C" const char* g_FableEulaDocument_01374F54;
extern "C" const char* g_FableWarrantyDocument_01374F58;

extern "C" void* __fastcall FableEulaConvertLanguage_00401fe0(
    void*,
    void*,
    void*);
extern "C" void __fastcall FableEulaDestroyLanguage_00401fe0(void*, void*);

__declspec(naked) int FABLE_FASTCALL EULA::DoEULAThings()
{
    __asm
    {
        push ecx
        push edi
        push offset kEulaLibraryName
        call dword ptr [g_FableEulaLoadLibraryW_00401fe0]
        mov edi, eax
        test edi, edi
        jnz library_loaded
        pop edi
        pop ecx
        ret

    library_loaded:
        push esi
        push offset kEulaExportName
        push edi
        call dword ptr [g_FableEulaGetProcAddress_00401fe0]
        mov esi, eax
        test esi, esi
        jz failure
        lea eax, dword ptr [esp + 8]
        push eax
        mov ecx, offset g_FableEulaLanguage_013BCA28
        call FableEulaConvertLanguage_00401fe0
        mov eax, dword ptr [eax]
        test eax, eax
        jnz language_present
        mov eax, offset g_FableEulaEmptyText_0129AAF4
        jmp language_ready

    language_present:
        mov eax, dword ptr [eax]

    language_ready:
        mov ecx, dword ptr [g_FableWarrantyDocument_01374F58]
        mov edx, dword ptr [g_FableEulaDocument_01374F54]
        push ebx
        push 1
        push ecx
        push edx
        push eax
        call esi
        mov ebx, eax
        dec ebx
        neg ebx
        sbb bl, bl
        lea ecx, dword ptr [esp + 0Ch]
        inc bl
        call FableEulaDestroyLanguage_00401fe0
        test bl, bl
        pop ebx
        jz failure
        push edi
        call dword ptr [g_FableEulaFreeLibrary_00401fe0]
        pop esi
        mov eax, 1
        pop edi
        pop ecx
        ret

    failure:
        pop esi
        xor eax, eax
        pop edi
        pop ecx
        ret
    }
}
