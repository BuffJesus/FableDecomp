// NUISystem::CFrontEndManager::GotoProfileMenu @ 0x00597B20
// VC7.1, x86, /O2 /Oy.
//
// The profile route refreshes availability, then either replaces used-key
// 0x0b at the top of a multi-entry stack with key 7 (profiles exist) or key
// 0x0c (none exist), or delegates key 7 to GotoNextScreen(false).  The direct
// replacement path transfers outgoing component ownership, publishes
// +0x98/+0x9c, and pushes the replacement.  Sound/refcount work exists only
// on the delegated GotoNextScreen path; retail has no direct sound call here.

extern "C" long __fastcall
FableFrontEndGotoProfileRefreshProfiles(void* manager, void*);
extern "C" unsigned long __fastcall
FableFrontEndGotoProfileStackSize(
    void* stackEnd,
    void*,
    void* stack);
extern "C" long** __fastcall
FableFrontEndGotoProfileResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" void** __fastcall
FableFrontEndGotoProfileStackTop(void* stack, void*);
extern "C" void* __cdecl FableFrontEndGotoProfileGetManager();
extern "C" void __fastcall
FableFrontEndGotoProfileStackPop(void* stack, void*);
extern "C" void __fastcall
FableFrontEndGotoProfileStackPush(
    void* stack,
    void*,
    void** component);
extern "C" void __fastcall
FableFrontEndGotoProfileNext(
    void* manager,
    void*,
    void* component,
    unsigned char transition);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GotoProfileMenu_00597b20(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push ecx
        push ebx
        push esi
        push edi
        mov esi, ecx
        call FableFrontEndGotoProfileRefreshProfiles
        lea ebx, [esi + 20h]
        push ebx
        lea ecx, [ebx + 10h]
        mov dword ptr [ebp - 8], eax
        call FableFrontEndGotoProfileStackSize
        cmp eax, 1
        jbe delegateProfile
        lea eax, [ebp - 4]
        lea ecx, [esi + 54h]
        push eax
        mov dword ptr [ebp - 4], 0bh
        call FableFrontEndGotoProfileResolveUsedKey
        mov ecx, ebx
        mov edi, eax
        call FableFrontEndGotoProfileStackTop
        mov eax, dword ptr [eax]
        cmp eax, dword ptr [edi]
        jne delegateProfile
        mov ecx, ebx
        call FableFrontEndGotoProfileStackTop
        mov edi, dword ptr [eax]
        mov eax, dword ptr [edi]
        push 6
        mov ecx, edi
        call dword ptr [eax + 0c0h]
        call FableFrontEndGotoProfileGetManager
        mov edx, dword ptr [eax]
        lea ecx, [edi + 4]
        push ecx
        mov ecx, eax
        call dword ptr [edx + 14h]
        mov ecx, ebx
        call FableFrontEndGotoProfileStackPop
        cmp dword ptr [ebp - 8], 0
        lea eax, [ebp - 8]
        lea ecx, [esi + 54h]
        push eax
        mov dword ptr [ebp - 8], 7
        jne profileKeyReady
        mov dword ptr [ebp - 8], 0ch

profileKeyReady:
        call FableFrontEndGotoProfileResolveUsedKey
        mov eax, dword ptr [eax]
        lea ecx, [esi + 9ch]
        mov dword ptr [ecx], eax
        push ecx
        mov ecx, ebx
        mov dword ptr [esi + 98h], edi
        call FableFrontEndGotoProfileStackPush
        jmp done

delegateProfile:
        lea eax, [ebp - 8]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 8], 7
        call FableFrontEndGotoProfileResolveUsedKey
        push 0
        push dword ptr [eax]
        mov ecx, esi
        call FableFrontEndGotoProfileNext

done:
        pop edi
        pop esi
        pop ebx
        leave
        ret
    }
}
