// NUISystem::CFrontEndManager::AddPrefixToTitleBar @ 0x00595AD9
// VC7.1, x86, /O2 /Oy.
//
// The selected screen is resolved through the manager's used-key map.  If it
// exists, retail copies the requested child name into the virtual lookup and
// resets the returned text component with the supplied wide-string prefix.

extern "C" void** __fastcall
FableFrontEndTitlePrefixResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* screen);
extern "C" void __fastcall
FableFrontEndTitlePrefixCopyName(
    void* destination,
    void*,
    const void* source);
extern "C" void __fastcall
FableFrontEndTitlePrefixResetText(
    void* text,
    void*,
    const void* prefix);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_AddPrefixToTitleBar_00595ad9(
    void*,
    void*,
    unsigned long,
    const void*,
    const void*)
{
    __asm
    {
        push esi
        push edi
        lea esi, [ecx + 54h]
        lea eax, [esp + 0ch]
        push eax
        mov ecx, esi
        call FableFrontEndTitlePrefixResolveUsedKey
        cmp dword ptr [eax], 0
        je done
        lea eax, [esp + 0ch]
        push eax
        mov ecx, esi
        call FableFrontEndTitlePrefixResolveUsedKey
        mov esi, dword ptr [eax]
        mov edi, dword ptr [esi]
        push ecx
        mov ecx, esp
        push dword ptr [esp + 14h]
        call FableFrontEndTitlePrefixCopyName
        mov ecx, esi
        call dword ptr [edi + 0ch]
        test eax, eax
        je done
        push dword ptr [esp + 14h]
        mov ecx, eax
        call FableFrontEndTitlePrefixResetText

done:
        pop edi
        pop esi
        ret 0ch
    }
}
