#include "fable_string.h"

extern "C" void FABLE_CDECL
FableWideLiteralFree_0099B800(void* memory);
extern "C" void FABLE_CDECL
FableWideLiteralDelete_0099B800(void* memory);
extern "C" void* FABLE_FASTCALL
FableWideLiteralAllocateData_0099B800(
    void* self,
    void*,
    const wchar_t* text,
    long length);

// CWideString::operator=(wchar_t const*) @ 0x0099B800.
__declspec(naked) const CWideString&
CWideString::operator=(const wchar_t*)
{
    __asm
    {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi]
        test eax, eax
        je old_storage_released
        mov edx, dword ptr [eax + 0Ch]
        dec edx
        mov ecx, edx
        test ecx, ecx
        mov dword ptr [eax + 0Ch], edx
        jg clear_old_storage
        push edi
        mov edi, dword ptr [esi]
        test edi, edi
        je old_data_deleted
        mov eax, dword ptr [edi]
        test eax, eax
        je delete_old_data
        push eax
        call FableWideLiteralFree_0099B800
        add esp, 4

    delete_old_data:
        push edi
        call FableWideLiteralDelete_0099B800
        add esp, 4

    old_data_deleted:
        pop edi

    clear_old_storage:
        mov dword ptr [esi], 0

    old_storage_released:
        mov eax, dword ptr [esp + 8]
        test eax, eax
        je finished
        cmp word ptr [eax], 0
        je finished
        push -1
        push eax
        mov ecx, esi
        call FableWideLiteralAllocateData_0099B800
        mov dword ptr [esi], eax

    finished:
        mov eax, esi
        pop esi
        ret 4
    }
}
