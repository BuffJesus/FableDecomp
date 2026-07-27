#include "rebuild_abi.h"

struct FableStringParser_00404720
{
    unsigned char bytes[0x18];
};

extern "C" void FABLE_FASTCALL
FableStringParserBaseConstruct_0099A2D0(void* self, void*);
extern "C" void FABLE_FASTCALL
FableCharStringConstruct_0099E4B0(void* self, void*);
extern "C" void* FABLE_CDECL
FableAllocate_00BFEA0E(unsigned long size);
extern "C" void* FABLE_FASTCALL
FableCharStringAssign_0099EFB0(
    void* self,
    void*,
    const void* source);
extern "C" void FABLE_FASTCALL
FableStringParserInitialize_009B9530(
    void* self,
    void*,
    const char* text,
    void* errorCallback,
    void* sourceName,
    long mode);

// CStringParser::CStringParser(
//     CCharString const&,
//     void (*)(CCharString*),
//     CCharString const&,
//     long) @ 0x00404720.
//
// The retail constructor builds the parser base, two string members, and the
// circular list sentinel at +0x14 before forwarding the resolved input buffer
// and caller metadata to CStringParser::Initialize.
extern "C" __declspec(naked) FableStringParser_00404720* FABLE_FASTCALL
FableStringParserConstruct_00404720(
    FableStringParser_00404720* self,
    void*,
    const void* input,
    void* errorCallback,
    void* sourceName,
    long mode)
{
    __asm
    {
        push ebx
        push esi
        push edi
        mov esi, ecx
        call FableStringParserBaseConstruct_0099A2D0
        lea ecx, [esi + 0Ch]
        mov dword ptr [esi], 0122D874h
        call FableCharStringConstruct_0099E4B0
        lea edi, [esi + 10h]
        mov ecx, edi
        call FableCharStringConstruct_0099E4B0
        push 10h
        mov dword ptr [esi + 14h], 0
        call FableAllocate_00BFEA0E
        mov ebx, dword ptr [esp + 14h]
        add esp, 4
        mov dword ptr [eax], eax
        mov dword ptr [eax + 4], eax
        push ebx
        mov ecx, edi
        mov dword ptr [esi + 14h], eax
        call FableCharStringAssign_0099EFB0
        mov eax, dword ptr [ebx]
        test eax, eax
        jne haveInput
        mov eax, 0129AAF4h
        jmp resolvedInput

haveInput:
        mov eax, dword ptr [eax]

resolvedInput:
        mov ecx, dword ptr [esp + 1Ch]
        mov edx, dword ptr [esp + 18h]
        push ecx
        mov ecx, dword ptr [esp + 18h]
        push edx
        push ecx
        push eax
        mov ecx, esi
        call FableStringParserInitialize_009B9530
        pop edi
        mov eax, esi
        pop esi
        pop ebx
        ret 10h
    }
}
