// NUISystem::CFrontEndManager::GotoPreviousScreen @ 0x00597BF2
// VC7.1, x86, /O2 /Oy.
//
// PDB ABI:
//   void __thiscall GotoPreviousScreen(bool)
//
// This preserves the complete retail dispatcher, including protected root
// key 0x14, forced key 0x1a fallback, key 0x1b/key 7 special navigation,
// state-6 and ownership transfer of the outgoing component, stack pop and
// +0x98/+0x9c publication, key 0x17 virtual-keyboard ownership, and back
// sound +0x19c with intrusive release.

extern "C" void** __fastcall
FableFrontEndGotoPreviousStackTop(void* stack, void*);
extern "C" long** __fastcall
FableFrontEndGotoPreviousResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" void* __cdecl FableFrontEndGotoPreviousGetProfileManager();
extern "C" void __fastcall
FableFrontEndGotoPreviousLoadProfileManager(void* profileManager, void*);
extern "C" void __fastcall
FableFrontEndGotoPreviousSynchronizeGame(void* game, void*);
extern "C" void __fastcall
FableFrontEndGotoPreviousFinishDynamicTextures(
    void* manager,
    void*,
    int immediate);
extern "C" unsigned long __fastcall
FableFrontEndGotoPreviousStackSize(
    void* stackEnd,
    void*,
    void* stack);
extern "C" unsigned char __fastcall
FableFrontEndGotoPreviousIterateElements(
    void* manager,
    void*,
    void* component);
extern "C" void __fastcall
FableFrontEndGotoPreviousSpecific(
    void* manager,
    void*,
    void* component);
extern "C" void __fastcall
FableFrontEndGotoPreviousStackPop(void* stack, void*);
extern "C" void* __cdecl FableFrontEndGotoPreviousGetManager();
extern "C" void* __cdecl
FableFrontEndGotoPreviousNew(unsigned long bytes);
extern "C" void* __fastcall
FableFrontEndGotoPreviousKeyboardConstruct(
    void* storage,
    void*,
    void* component);
extern "C" void __fastcall
FableFrontEndGotoPreviousKeyboardInitialize(void* keyboard, void*);
extern "C" void __fastcall
FableFrontEndGotoPreviousNext(
    void* manager,
    void*,
    void* component,
    unsigned char transition);

extern "C" void* FableFrontEndGotoPreviousGame;

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GotoPreviousScreen_00597bf2(
    void*,
    void*,
    unsigned char)
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
        mov edi, ecx
        lea ebx, [edi + 20h]
        mov ecx, ebx
        call FableFrontEndGotoPreviousStackTop
        mov eax, dword ptr [eax]
        mov dword ptr [ebp - 8], eax
        lea eax, [ebp - 4]
        lea esi, [edi + 54h]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 4], 14h
        call FableFrontEndGotoPreviousResolveUsedKey
        mov ecx, dword ptr [ebp - 8]
        cmp ecx, dword ptr [eax]
        jne normalBack
        cmp byte ptr [ebp + 8], 0
        je done
        jmp forcedFallback

normalBack:
        call FableFrontEndGotoPreviousGetProfileManager
        mov ecx, eax
        call FableFrontEndGotoPreviousLoadProfileManager
        mov ecx, dword ptr [FableFrontEndGotoPreviousGame]
        call FableFrontEndGotoPreviousSynchronizeGame
        push 0
        mov ecx, edi
        call FableFrontEndGotoPreviousFinishDynamicTextures
        push ebx
        lea ecx, [ebx + 10h]
        call FableFrontEndGotoPreviousStackSize
        cmp eax, 1
        jbe shallowStack
        lea eax, [ebp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [ebp + 8], 1bh
        call FableFrontEndGotoPreviousResolveUsedKey
        mov ecx, ebx
        mov dword ptr [ebp + 8], eax
        call FableFrontEndGotoPreviousStackTop
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [ebp + 8]
        cmp eax, dword ptr [ecx]
        jne popCurrent
        push 7
        pop ebx
        lea eax, [ebp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [ebp + 8], ebx
        call FableFrontEndGotoPreviousResolveUsedKey
        push dword ptr [eax]
        mov ecx, edi
        call FableFrontEndGotoPreviousIterateElements
        test al, al
        lea eax, [ebp + 8]
        mov ecx, esi
        push eax
        je specialGotoNext
        mov dword ptr [ebp + 8], ebx
        call FableFrontEndGotoPreviousResolveUsedKey
        push dword ptr [eax]
        mov ecx, edi
        call FableFrontEndGotoPreviousSpecific
        jmp playBackSound

specialGotoNext:
        mov dword ptr [ebp + 8], 14h
        call FableFrontEndGotoPreviousResolveUsedKey
        push 1
        jmp callNext

popCurrent:
        mov ecx, ebx
        call FableFrontEndGotoPreviousStackTop
        mov esi, dword ptr [eax]
        mov eax, dword ptr [esi]
        push 6
        mov ecx, esi
        call dword ptr [eax + 0c0h]
        call FableFrontEndGotoPreviousGetManager
        mov edx, dword ptr [eax]
        lea ecx, [esi + 4]
        push ecx
        mov ecx, eax
        call dword ptr [edx + 14h]
        mov ecx, ebx
        call FableFrontEndGotoPreviousStackPop
        mov ecx, ebx
        call FableFrontEndGotoPreviousStackTop
        mov eax, dword ptr [eax]
        mov dword ptr [edi + 9ch], eax
        mov dword ptr [edi + 98h], esi
        lea eax, [ebp + 8]
        lea esi, [edi + 54h]
        push eax
        mov ecx, esi
        mov dword ptr [ebp + 8], 17h
        call FableFrontEndGotoPreviousResolveUsedKey
        mov ecx, ebx
        mov dword ptr [ebp + 8], eax
        call FableFrontEndGotoPreviousStackTop
        mov eax, dword ptr [eax]
        mov ecx, dword ptr [ebp + 8]
        cmp eax, dword ptr [ecx]
        jne playBackSound
        call FableFrontEndGotoPreviousGetProfileManager
        push 10h
        mov byte ptr [eax + 0ch], 1
        call FableFrontEndGotoPreviousNew
        mov ebx, eax
        test ebx, ebx
        pop ecx
        je keyboardNull
        lea eax, [ebp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [ebp + 8], 17h
        call FableFrontEndGotoPreviousResolveUsedKey
        push dword ptr [eax]
        mov ecx, ebx
        call FableFrontEndGotoPreviousKeyboardConstruct
        jmp keyboardReady

keyboardNull:
        xor eax, eax

keyboardReady:
        mov ecx, eax
        mov dword ptr [edi + 60h], eax
        call FableFrontEndGotoPreviousKeyboardInitialize
        jmp playBackSound

shallowStack:
        cmp byte ptr [ebp + 8], 0
        je playBackSound

forcedFallback:
        lea eax, [ebp + 8]
        mov ecx, esi
        push eax
        mov dword ptr [ebp + 8], 1ah
        call FableFrontEndGotoPreviousResolveUsedKey
        push 0

callNext:
        push dword ptr [eax]
        mov ecx, edi
        call FableFrontEndGotoPreviousNext

playBackSound:
        call FableFrontEndGotoPreviousGetManager
        mov esi, eax
        call FableFrontEndGotoPreviousGetManager
        lea ecx, [ebp + 8]
        push ecx
        mov edi, eax
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [edi]
        add eax, 19ch
        push 1
        mov ecx, edi
        push eax
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [ebp + 8]
        test eax, eax
        je done
        mov ecx, eax
        add eax, 4
        dec dword ptr [eax]
        cmp dword ptr [eax], 0
        jne done
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 4]

done:
        pop edi
        pop esi
        pop ebx
        leave
        ret 4
    }
}
