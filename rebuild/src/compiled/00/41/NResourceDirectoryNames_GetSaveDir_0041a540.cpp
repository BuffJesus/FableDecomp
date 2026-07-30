#include "fable_resource_directories.h"

namespace
{
    const wchar_t kFableSaveRoot[] = L"My Games\\Fable\\";
    const wchar_t kFableSaveLeaf[] = L"Saves";
    const wchar_t kTrailingBackslash[] = L"\\";
}

extern "C" void* FABLE_FASTCALL
FableGetMyDocumentsDir_0041A540(void* destination, void*);
extern "C" void* FABLE_FASTCALL
FableAppendWideLiteral_0041A540(
    void* destination,
    void* left,
    const wchar_t* right);
extern "C" void FABLE_FASTCALL
FableDestroyWideTemporary_0041A540(void* value, void*);

__declspec(naked) CWideString FABLE_FASTCALL
NResourceDirectoryNames::GetSaveDir()
{
    __asm
    {
        sub esp, 0Ch
        push esi
        mov esi, ecx
        push offset kFableSaveRoot
        lea ecx, dword ptr [esp + 10h]
        call FableGetMyDocumentsDir_0041A540
        mov edx, eax
        lea ecx, dword ptr [esp + 8]
        call FableAppendWideLiteral_0041A540
        lea ecx, dword ptr [esp + 0Ch]
        call FableDestroyWideTemporary_0041A540
        push offset kFableSaveLeaf
        lea edx, dword ptr [esp + 8]
        lea ecx, dword ptr [esp + 0Ch]
        call FableAppendWideLiteral_0041A540
        lea ecx, dword ptr [esp + 4]
        call FableDestroyWideTemporary_0041A540
        push offset kTrailingBackslash
        lea edx, dword ptr [esp + 0Ch]
        mov ecx, esi
        call FableAppendWideLiteral_0041A540
        lea ecx, dword ptr [esp + 8]
        call FableDestroyWideTemporary_0041A540
        mov eax, esi
        pop esi
        add esp, 0Ch
        ret
    }
}
