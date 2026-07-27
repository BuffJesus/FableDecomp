#include "rebuild_abi.h"

struct FablePersistContext_009BADD0
{
    void* definitionManager;
    void* symbolMap;
    void* parser;
    void* errorCallback;
    void* sourceName;
    unsigned char reportErrors;
    unsigned char field15;
    unsigned char field16;
    unsigned char field17;
    unsigned long mode;
};

// CPersistContext::CPersistContext(
//     CStringParser*,
//     void (*)(CCharString*),
//     bool,
//     CDefinitionManager*,
//     CSymbolMap*,
//     CCharString*) @ 0x009BADD0.
//
// This is the text-load context used by GFMain Phase 3 after constructing the
// settings parser. The retail constructor stores the two manager pointers
// first, then the parser/callback/source tuple, and clears all runtime state.
extern "C" __declspec(naked) FablePersistContext_009BADD0* FABLE_FASTCALL
FablePersistContextTextConstruct_009BADD0(
    FablePersistContext_009BADD0* self,
    void*,
    void* parser,
    void* errorCallback,
    bool reportErrors,
    void* definitionManager,
    void* symbolMap,
    void* sourceName)
{
    __asm
    {
        mov edx, dword ptr [esp + 10h]
        mov eax, ecx
        mov dword ptr [eax], edx
        mov edx, dword ptr [esp + 14h]
        mov dword ptr [eax + 4], edx
        mov edx, dword ptr [esp + 4]
        mov dword ptr [eax + 8], edx
        mov edx, dword ptr [esp + 8]
        mov dword ptr [eax + 0Ch], edx
        mov edx, dword ptr [esp + 18h]
        xor ecx, ecx
        mov dword ptr [eax + 10h], edx
        mov dl, byte ptr [esp + 0Ch]
        mov dword ptr [eax + 18h], ecx
        mov byte ptr [eax + 14h], dl
        mov byte ptr [eax + 15h], cl
        mov byte ptr [eax + 17h], cl
        mov byte ptr [eax + 16h], cl
        ret 18h
    }
}
