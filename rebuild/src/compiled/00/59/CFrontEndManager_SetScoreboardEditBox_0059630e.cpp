// NUISystem::CFrontEndManager::SetScoreboardEditBox @ 0x0059630E
// VC7.1, x86, /O2 /Oy.
//
// The scoreboard screen is used-key 0x19.  Retail resolves the named child,
// requires component type 0x25, sets its wide value, refreshes it, and
// deactivates it with flag zero.

extern "C" void** __fastcall
FableFrontEndScoreboardSetResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" void __fastcall
FableFrontEndScoreboardSetCopyName(
    void* destination,
    void*,
    const void* source);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_SetScoreboardEditBox_0059630e(
    void*,
    void*,
    const void*,
    const void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        push edi
        lea eax, [ebp - 4]
        push eax
        add ecx, 54h
        mov dword ptr [ebp - 4], 19h
        call FableFrontEndScoreboardSetResolveUsedKey
        mov esi, dword ptr [eax]
        mov edi, dword ptr [esi]
        push ecx
        mov ecx, esp
        push dword ptr [ebp + 8]
        call FableFrontEndScoreboardSetCopyName
        mov ecx, esi
        call dword ptr [edi + 0ch]
        mov esi, eax
        test esi, esi
        je done
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 104h]
        cmp eax, 25h
        jne done
        push dword ptr [ebp + 0ch]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 23ch]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 24ch]
        mov eax, dword ptr [esi]
        push 0
        mov ecx, esi
        call dword ptr [eax + 258h]

done:
        pop edi
        pop esi
        leave
        ret 8
    }
}
