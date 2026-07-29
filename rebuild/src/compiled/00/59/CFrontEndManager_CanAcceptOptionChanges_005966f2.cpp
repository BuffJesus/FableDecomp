// NUISystem::CFrontEndManager::CanAcceptOptionChanges @ 0x005966F2
// VC7.1, x86, /O2 /Oy.
//
// Option changes are gated only while used-key 0x16 is the current front-end
// component.  Other screens accept immediately; the options screen delegates
// to the redefiner/change validator.

extern "C" long** __fastcall
FableFrontEndCanAcceptResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" long** __fastcall
FableFrontEndCanAcceptStackTop(void* stack, void*);
extern "C" unsigned char __cdecl
FableFrontEndCanAcceptChanges();

extern "C" __declspec(naked) unsigned char __fastcall
CFrontEndManager_CanAcceptOptionChanges_005966f2(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        push edi
        mov esi, ecx
        lea eax, [ebp - 4]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 4], 16h
        call FableFrontEndCanAcceptResolveUsedKey
        lea ecx, [esi + 20h]
        mov edi, eax
        call FableFrontEndCanAcceptStackTop
        mov eax, dword ptr [eax]
        cmp eax, dword ptr [edi]
        pop edi
        pop esi
        jne accept
        call FableFrontEndCanAcceptChanges
        leave
        ret

accept:
        mov al, 1
        leave
        ret
    }
}
