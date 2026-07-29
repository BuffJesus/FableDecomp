// NUISystem::CFrontEndManager::GetInstance @ 0x00595582
// VC7.1, x86, /O2 /Oy.
//
// Lazy singleton construction allocates the exact 0xe0-byte manager object,
// invokes its constructor on success, stores the returned pointer, and leaves
// the singleton null when allocation fails.

extern "C" void* __cdecl
FableFrontEndManagerAllocate(unsigned long bytes);
extern "C" void* __fastcall
FableFrontEndManagerConstructInstance(void* storage, void*);

extern "C" void* FableFrontEndManagerSingleton;

extern "C" __declspec(naked) void* __fastcall
CFrontEndManager_GetInstance_00595582()
{
    __asm
    {
        mov eax, dword ptr [FableFrontEndManagerSingleton]
        test eax, eax
        jne done
        push 0e0h
        call FableFrontEndManagerAllocate
        test eax, eax
        pop ecx
        je allocationFailed
        mov ecx, eax
        call FableFrontEndManagerConstructInstance
        jmp publish

allocationFailed:
        xor eax, eax

publish:
        mov dword ptr [FableFrontEndManagerSingleton], eax

done:
        ret
    }
}
