// NUISystem::CFrontEndManager::SetMainMenuDefName @ 0x00595A06
// VC7.1, x86, /O2 /Oy.
//
// Used-key zero owns the main-menu component.  Matching definitions retain
// the current component; differing definitions delete it with flag one and
// recreate it.  An absent component is created with front-end flag zero and
// activated through virtual slot 0xac.

extern "C" void** __fastcall
FableFrontEndMainMenuResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" unsigned char __fastcall
FableFrontEndMainMenuDefinitionDiffers(void* comparison, void*);
extern "C" void __fastcall
FableFrontEndMainMenuDestroyTemporary(void* temporary, void*);
extern "C" void* __cdecl
FableFrontEndMainMenuGetManager();
extern "C" void* __fastcall
FableFrontEndMainMenuCreateComponent(
    void* manager,
    void*,
    const void* definition,
    long frontEnd);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_SetMainMenuDefName_00595a06(
    void*,
    void*,
    const void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 0ch
        push ebx
        push esi
        lea esi, [ecx + 54h]
        lea eax, [ebp - 8]
        xor ebx, ebx
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 8], ebx
        call FableFrontEndMainMenuResolveUsedKey
        cmp dword ptr [eax], ebx
        je ensureComponent
        lea eax, [ebp - 8]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 8], ebx
        call FableFrontEndMainMenuResolveUsedKey
        push dword ptr [ebp + 8]
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 0ch]
        push edx
        call dword ptr [eax + 150h]
        mov ecx, eax
        call FableFrontEndMainMenuDefinitionDiffers
        lea ecx, [ebp - 0ch]
        mov byte ptr [ebp - 1], al
        call FableFrontEndMainMenuDestroyTemporary
        cmp byte ptr [ebp - 1], bl
        je ensureComponent
        lea eax, [ebp - 0ch]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 0ch], ebx
        call FableFrontEndMainMenuResolveUsedKey
        mov ecx, dword ptr [eax]
        cmp ecx, ebx
        je clearComponent
        mov eax, dword ptr [ecx]
        push 1
        call dword ptr [eax]

clearComponent:
        lea eax, [ebp - 0ch]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 0ch], ebx
        call FableFrontEndMainMenuResolveUsedKey
        mov dword ptr [eax], ebx

ensureComponent:
        lea eax, [ebp - 0ch]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 0ch], ebx
        call FableFrontEndMainMenuResolveUsedKey
        cmp dword ptr [eax], ebx
        jne done
        push edi
        lea eax, [ebp - 0ch]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 0ch], ebx
        call FableFrontEndMainMenuResolveUsedKey
        push ebx
        push dword ptr [ebp + 8]
        mov edi, eax
        call FableFrontEndMainMenuGetManager
        mov ecx, eax
        call FableFrontEndMainMenuCreateComponent
        mov dword ptr [edi], eax
        lea eax, [ebp + 8]
        push eax
        mov ecx, esi
        mov dword ptr [ebp + 8], ebx
        call FableFrontEndMainMenuResolveUsedKey
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 0ach]
        pop edi

done:
        pop esi
        pop ebx
        leave
        ret 4
    }
}
