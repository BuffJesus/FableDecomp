// NUISystem::CFrontEndManager::CFrontEndManager @ 0x005953E2
// VC7.1, x86, /O2 /Oy.
//
// The constructor initializes the manager base, publishes the front-end
// vtable, constructs its string/list/container members, and clears the
// runtime screen/profile state through byte 0xd9.

extern "C" void __fastcall
FableFrontEndManagerConstructBase(void* manager, void*);
extern "C" void __fastcall
FableFrontEndManagerConstructString(void* string, void*);
extern "C" void __fastcall
FableFrontEndManagerConstructList(
    void* list,
    void*,
    void* allocator);
extern "C" void __fastcall
FableFrontEndManagerConstructStack(void* stack, void*);
extern "C" void __fastcall
FableFrontEndManagerConstructUsedKeys(void* usedKeys, void*);
extern "C" void __fastcall
FableFrontEndManagerConstructTail(void* tail, void*);

extern "C" void* const FableFrontEndManagerVtable[];

extern "C" __declspec(naked) void* __fastcall
CFrontEndManager_CFrontEndManager_005953e2(void*, void*)
{
    __asm
    {
        push ecx
        push ebx
        push esi
        mov esi, ecx
        call FableFrontEndManagerConstructBase
        lea ecx, [esi + 14h]
        mov dword ptr [esi], offset FableFrontEndManagerVtable
        call FableFrontEndManagerConstructString
        lea eax, [esp + 0bh]
        push eax
        lea ecx, [esi + 18h]
        call FableFrontEndManagerConstructList
        lea ecx, [esi + 20h]
        call FableFrontEndManagerConstructStack
        xor ebx, ebx
        mov dword ptr [esi + 48h], ebx
        mov dword ptr [esi + 4ch], ebx
        lea ecx, [esi + 54h]
        mov dword ptr [esi + 50h], ebx
        call FableFrontEndManagerConstructUsedKeys
        mov dword ptr [esi + 60h], ebx
        mov byte ptr [esi + 64h], bl
        mov dword ptr [esi + 68h], ebx
        mov dword ptr [esi + 6ch], ebx
        mov dword ptr [esi + 70h], ebx
        mov dword ptr [esi + 74h], ebx
        mov dword ptr [esi + 78h], ebx
        mov dword ptr [esi + 7ch], ebx
        lea eax, [esi + 80h]
        mov dword ptr [eax], ebx
        mov dword ptr [eax + 4], ebx
        mov dword ptr [eax + 8], ebx
        lea eax, [esi + 8ch]
        mov dword ptr [eax], ebx
        mov dword ptr [eax + 4], ebx
        mov dword ptr [eax + 8], ebx
        lea ecx, [esi + 0ach]
        mov dword ptr [esi + 98h], ebx
        mov dword ptr [esi + 9ch], ebx
        mov byte ptr [esi + 0a0h], bl
        mov dword ptr [esi + 0a4h], ebx
        mov dword ptr [esi + 0a8h], ebx
        call FableFrontEndManagerConstructString
        lea ecx, [esi + 0b0h]
        call FableFrontEndManagerConstructString
        lea ecx, [esi + 0c4h]
        mov byte ptr [esi + 0c0h], bl
        call FableFrontEndManagerConstructTail
        mov byte ptr [esi + 0d8h], bl
        mov byte ptr [esi + 0d9h], bl
        mov eax, esi
        pop esi
        pop ebx
        pop ecx
        ret
    }
}
