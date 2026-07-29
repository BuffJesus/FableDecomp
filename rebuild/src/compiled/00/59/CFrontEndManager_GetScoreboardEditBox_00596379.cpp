// NUISystem::CFrontEndManager::GetScoreboardEditBox @ 0x00596379
// VC7.1, x86, /O2 /Oy.
//
// The scoreboard screen is used-key 0x19.  A named child of type 0x25 writes
// its value into the hidden wide-string return object through slot 0x238;
// missing or wrong-type children default-construct that return object.

extern "C" void** __fastcall
FableFrontEndScoreboardGetResolveUsedKey(
    void* usedKeys,
    void*,
    const unsigned long* key);
extern "C" void __fastcall
FableFrontEndScoreboardGetCopyName(
    void* destination,
    void*,
    const void* source);
extern "C" void __fastcall
FableFrontEndScoreboardGetConstructDefault(void* output, void*);

extern "C" __declspec(naked) void* __fastcall
CFrontEndManager_GetScoreboardEditBox_00596379(
    void*,
    void*,
    void*,
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
        call FableFrontEndScoreboardGetResolveUsedKey
        mov esi, dword ptr [eax]
        mov edi, dword ptr [esi]
        push ecx
        mov ecx, esp
        push dword ptr [ebp + 0ch]
        call FableFrontEndScoreboardGetCopyName
        mov ecx, esi
        call dword ptr [edi + 0ch]
        mov esi, eax
        test esi, esi
        je constructDefault
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 104h]
        cmp eax, 25h
        jne constructDefault
        push dword ptr [ebp + 8]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 238h]
        jmp returnOutput

constructDefault:
        mov ecx, dword ptr [ebp + 8]
        call FableFrontEndScoreboardGetConstructDefault

returnOutput:
        mov eax, dword ptr [ebp + 8]
        pop edi
        pop esi
        leave
        ret 8
    }
}
