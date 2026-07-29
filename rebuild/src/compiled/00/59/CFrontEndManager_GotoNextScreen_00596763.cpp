// NUISystem::CFrontEndManager::GotoNextScreen @ 0x00596763
// VC7.1, x86, /O2 /Oy.
//
// PDB ABI:
//   void __thiscall GotoNextScreen(CComponent*, bool)
//
// The body retains the retail screen-stack ordering, used-key 0x1a filter,
// component state/ownership calls, transition stack-clear flag, published
// old/new fields (+0x98/+0x9c), forward sound (+0x1a0), intrusive release,
// and the retail reuse of the bool argument slot as the CDefPointer local.

extern "C" void** __fastcall
FableFrontEndGotoNextStackTop(void* stack, void*);
extern "C" void __fastcall
FableFrontEndGotoNextDeactivateEditBoxes(
    void* manager,
    void*,
    void* component);
extern "C" void** __fastcall
FableFrontEndGotoNextResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" long __fastcall
FableFrontEndGotoNextStackSize(
    void* stackEnd,
    void*,
    void* stack);
extern "C" void __fastcall
FableFrontEndGotoNextStackPop(void* stack, void*);
extern "C" void __fastcall
FableFrontEndGotoNextStackPush(
    void* stack,
    void*,
    void** component);
extern "C" void* __cdecl FableFrontEndGotoNextGetManager();

extern "C" void* FableFrontEndGotoNextWorld;
extern "C" const float FableFrontEndGotoNextEffectScale;

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_GotoNextScreen_00596763(
    void*,
    void*,
    void*,
    unsigned char)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push ebx
        push esi
        mov esi, ecx
        lea ebx, [esi + 20h]
        push edi
        mov ecx, ebx
        call FableFrontEndGotoNextStackTop
        mov edi, dword ptr [eax]
        push 0
        mov ecx, esi
        call FableFrontEndGotoNextDeactivateEditBoxes
        lea eax, [ebp - 4]
        push eax
        lea ecx, [esi + 54h]
        mov dword ptr [ebp - 4], 1ah
        call FableFrontEndGotoNextResolveUsedKey
        mov ecx, dword ptr [ebp + 8]
        cmp ecx, dword ptr [eax]
        jne skipUsedKeyEffect
        mov ecx, dword ptr [FableFrontEndGotoNextWorld]
        test ecx, ecx
        je skipUsedKeyEffect
        fld dword ptr [FableFrontEndGotoNextEffectScale]
        mov eax, dword ptr [ecx]
        push ecx
        push ecx
        fstp dword ptr [esp + 4]
        fldz
        fstp dword ptr [esp]
        push 100h
        call dword ptr [eax + 0b8h]

skipUsedKeyEffect:
        mov eax, dword ptr [edi]
        push 6
        mov ecx, edi
        call dword ptr [eax + 0c0h]
        call FableFrontEndGotoNextGetManager
        mov edx, dword ptr [eax]
        lea ecx, [edi + 4]
        push ecx
        mov ecx, eax
        call dword ptr [edx + 14h]
        cmp byte ptr [ebp + 0ch], 0
        je pushTarget
        jmp queryStack

popStack:
        mov ecx, ebx
        call FableFrontEndGotoNextStackPop

queryStack:
        lea ecx, [ebx + 10h]
        push ebx
        call FableFrontEndGotoNextStackSize
        test eax, eax
        jne popStack

pushTarget:
        lea eax, [ebp + 8]
        push eax
        mov ecx, ebx
        call FableFrontEndGotoNextStackPush
        push 0
        mov ecx, esi
        call FableFrontEndGotoNextDeactivateEditBoxes
        mov eax, dword ptr [ebp + 8]
        mov dword ptr [esi + 9ch], eax
        mov dword ptr [esi + 98h], edi
        call FableFrontEndGotoNextGetManager
        mov esi, eax
        call FableFrontEndGotoNextGetManager
        lea ecx, [ebp + 0ch]
        mov edi, eax
        mov eax, dword ptr [esi]
        push ecx
        mov ecx, esi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [edi]
        push 1
        add eax, 1a0h
        push eax
        mov ecx, edi
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [ebp + 0ch]
        test eax, eax
        pop edi
        pop esi
        pop ebx
        je done
        mov ecx, eax
        add eax, 4
        dec dword ptr [eax]
        cmp dword ptr [eax], 0
        jne done
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 4]

done:
        leave
        ret 8
    }
}
