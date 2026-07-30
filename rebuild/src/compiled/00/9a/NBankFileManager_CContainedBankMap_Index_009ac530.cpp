#include "fable_retail_banks.h"

extern "C" void* FABLE_FASTCALL
FableContainedBankMapLowerBound_009AC530(
    NBankFileManager::CContainedBankMap* map,
    void*,
    const CCharString* key);
extern "C" bool FABLE_FASTCALL
FableContainedBankMapStringLess_009AC530(
    const void* leftData,
    void*,
    const void* rightData);
extern "C" void FABLE_FASTCALL
FableContainedBankMapStringCopy_009AC530(
    void* destination,
    void*,
    const CCharString* source);
extern "C" void* FABLE_FASTCALL
FableContainedBankMapInsert_009AC530(
    NBankFileManager::CContainedBankMap* map,
    void*,
    void** result,
    void* hint,
    const void* value);
extern "C" void FABLE_FASTCALL
FableContainedBankMapStringDestroy_009AC530(void* string, void*);

// The TLC build uses the VC7 three-word std::map representation. The donor
// PDB's CContainedBank layout and OpenRetailBank's five consecutive stores
// correct the propagated C2DVector specialization at this address.
__declspec(naked) NBankFileManager::CContainedBank&
NBankFileManager::CContainedBankMap::operator[](const CCharString&)
{
    __asm
    {
        sub esp, 30h
        push ebx
        mov ebx, dword ptr [esp + 38h]
        push ebp
        push esi
        push edi
        mov edi, ecx
        push ebx
        mov dword ptr [esp + 14h], edi
        call FableContainedBankMapLowerBound_009AC530
        mov edi, dword ptr [edi]
        mov esi, eax
        cmp esi, edi
        je insert_default
        mov eax, dword ptr [esi + 10h]
        mov ecx, dword ptr [ebx]
        cmp eax, ecx
        je found
        test ecx, ecx
        je insert_default
        test eax, eax
        je found
        push eax
        call FableContainedBankMapStringLess_009AC530
        test al, al
        je found

    insert_default:
        mov eax, dword ptr [esp + 44h]
        xor ebp, ebp
        push eax
        lea ecx, dword ptr [esp + 2Ch]
        xor edi, edi
        xor ebx, ebx
        mov dword ptr [esp + 28h], ebp
        call FableContainedBankMapStringCopy_009AC530
        lea edx, dword ptr [esp + 28h]
        xor eax, eax
        mov ecx, ebp
        push edx
        push ecx
        mov dword ptr [esp + 40h], eax
        mov eax, esp
        mov dword ptr [esp + 44h], ecx
        mov ecx, dword ptr [esp + 18h]
        mov dword ptr [esp + 34h], edi
        mov dword ptr [esp + 38h], ebx
        mov dword ptr [esp + 3Ch], ebp
        mov dword ptr [eax], esi
        lea eax, dword ptr [esp + 4Ch]
        push eax
        call FableContainedBankMapInsert_009AC530
        mov esi, dword ptr [esp + 44h]
        lea ecx, dword ptr [esp + 28h]
        call FableContainedBankMapStringDestroy_009AC530

    found:
        pop edi
        lea eax, dword ptr [esi + 14h]
        pop esi
        pop ebp
        pop ebx
        add esp, 30h
        ret 4
    }
}
